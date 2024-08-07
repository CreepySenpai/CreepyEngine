#include <ranges>
#include <CreepyEngine/Core/Application.hpp>
#include <CreepyEngine/Core/TimeStep.hpp>
#include <CreepyEngine/Events/ApplicationEvent.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>
#include <CreepyEngine/Scripting/ScriptEngine.hpp>
#include <GLFW/glfw3.h>

namespace Creepy {

    Application* Application::instance = nullptr;

    Application::Application(const std::string& title, uint32_t w, uint32_t h) noexcept {

        ENGINE_ASSERT((instance == nullptr), "Only one app instance exit!!!");

        // Because we create a pointer point to this obj, when we create obj we create it by smart pointer so when proc die it will
        // dealloc -> no memory leak when we use raw pointer
        // In imguilayer we get instance of this class so we need init this first in this program to dont be nullptr error
        instance = this;

        if(m_applicationDirectory.empty()){
            m_applicationDirectory = std::filesystem::current_path();
        }

        WindowProperty property{title, w, h};

        m_window = Window::Create(property);
        m_window->SetEventCallBack(std::bind_front(&Application::OnEvent, this));
        
        ENGINE_LOG_WARNING("Gona Create Render");
        Renderer::Init();
        // ENGINE_LOG_WARNING("Gona Create Script");
        // ScriptEngine::Init();
        // ENGINE_LOG_WARNING("Gona Create ImGui");
        // m_imGuiLayer = new ImGuiLayer();
        
        // this->PushOverlay(m_imGuiLayer);  // No memory leak because layerStack will free it
    }

    Application::~Application() noexcept {
        // ScriptEngine::ShutDown();
        ENGINE_LOG_WARNING("Call Shut Renderer");
        Renderer::ShutDown();
        ENGINE_LOG_WARNING("Call Shut Window");
        m_window->ShutDown();
        ENGINE_LOG_WARNING("Call Shut Down Eng");
    }

    void Application::Run() noexcept {

        while(m_isRunning){
            // Get current time
            const float time = static_cast<float>(glfwGetTime());

            // We get current time - last time -> delta time and save it
            TimeStep timeStep{time - m_lastFrameTime};

            // Save current time
            m_lastFrameTime = time;

            this->executeMainThreadQueue();

            // Save CPU render when we minimize window
            if(!m_minimized) {

                for(auto&& layer : m_layerStack){
                    layer->OnUpdate(timeStep);
                }
                
            }
            

            // Render All Data For ImGui Layer
            // m_imGuiLayer->Begin();

            // for(auto&& layer : m_layerStack){
            //     layer->OnImGuiRender();
            // }

            // m_imGuiLayer->End();

            m_window->OnUpdate(timeStep);

        }
        
    }

    void Application::Close() noexcept {
        m_isRunning = false;
    }

    void Application::OnEvent(Event& event) noexcept {
        EventDispatcher dispatcher{event};

        // We check all event, if event type == event have assign then we call it
        dispatcher.Dispatch<WindowCloseEvent>(std::bind_front(Application::OnWindowClose, this));

        dispatcher.Dispatch<WindowResizeEvent>(std::bind_front(Application::OnWindowResize, this));
        
        // ENGINE_LOG_INFO("CALL EVENT {}", event.ToString());

        for(auto&& layer : m_layerStack | std::views::reverse){
            layer->OnEvent(event);

            // If we set event was handle so we dont need pass event information to another layer, so if event is unique or only that func
            // handle, we need set == true, if not we set == false
            if(event.IsHandled()) {
                break;
            }

        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& event) noexcept {
        m_isRunning = false;

        return true;    // handled
    }

    bool Application::OnWindowResize(WindowResizeEvent& event) noexcept {
        if(event.GetWindowWidth() == 0 || event.GetWindowHeight() == 0) {
            m_minimized = true;
            return false;
        }

        m_minimized = false;

        Renderer::WindowResize(event.GetWindowWidth(), event.GetWindowHeight());

        return false;
    }

    void Application::PushLayer(Layer* layer) noexcept {
        m_layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PopLayer(Layer* layer) noexcept {
        m_layerStack.PopLayer(layer);
        layer->OnDetach();
    }

    void Application::PushOverlay(Layer* overlay) noexcept {
        m_layerStack.PushLayer(overlay);
        overlay->OnAttach();
    }


    void Application::PopOverlay(Layer* overlay) noexcept {
        m_layerStack.PopOverlay(overlay);
        overlay->OnDetach();
    }

    void Application::SubmitToMainThread(const std::function<void()>& func) noexcept {
        std::scoped_lock lock{m_mainThreadMutex};
        m_mainThreadQueue.emplace_back(func);
    }

    void Application::executeMainThreadQueue() noexcept {
        
        std::vector<std::function<void()>> copyQueue;
        {
            std::scoped_lock lock{m_mainThreadMutex};
            copyQueue.swap(m_mainThreadQueue);
        }

        for(auto& func : copyQueue){
            func();
        }

    }
}