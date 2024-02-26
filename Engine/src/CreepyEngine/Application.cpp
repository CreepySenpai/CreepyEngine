#include <CreepyEngine/Application.hpp>
#include <CreepyEngine/Input.hpp>
#include <CreepyEngine/MouseButtonCode.hpp>
#include <vector>
#include <ranges>
#include <iostream>
#include <GLFW/glfw3.h>

#include <CreepyEngine/Renderer/Renderer.hpp>


namespace Creepy {

    Application* Application::instance = nullptr;

    

    Application::Application() noexcept {

        if(instance){
            ENGINE_LOG_ERROR("Only Once Instance Exit");
        }

        // Because we create a pointer point to this obj, when we create obj we create it by smart pointer so when proc die it will
        // dealloc -> no memory leak when we use raw pointer
        // In imguilayer we get instance of this class so we need init this first in this program to dont be nullptr error
        instance = this;


        m_window = Window::Create();
        m_window->SetEventCallBack(std::bind_front(&Application::OnEvent, this));
        m_imGuiLayer = new ImGuiLayer();
        
        this->PushLayer(m_imGuiLayer);  // No memory leak because layerStack will free it

        Renderer::Init();
    }

    void Application::Run() noexcept {

        while(m_isRunning){
            // Get current time
            float time = static_cast<float>(glfwGetTime());

            // We get current time - last time -> delta time and save it
            TimeStep timeStep{time - m_lastFrameTime};

            // Save current time
            m_lastFrameTime = time;

            for(auto&& layer : m_layerStack){
                layer->OnUpdate(timeStep);
            }

            // Render All Data For ImGui Layer
            m_imGuiLayer->Begin();

            for(auto&& layer : m_layerStack){
                layer->OnImGuiRender();
            }

            m_imGuiLayer->End();

            m_window->OnUpdate();

        }
        
    }

    void Application::OnEvent(Event& event) noexcept {
        EventDispatcher dispatcher{event};

        // We check all event, if event type == event have assign then we call it
        dispatcher.Dispatch<WindowCloseEvent>(std::bind_front(Application::OnWindowClose, this));

        
        // ENGINE_LOG_INFO("CALL EVENT {}", event.ToString());

        // We iterator from top of stack
        for(auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); it++) {
            (*it)->OnEvent(event);

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
}