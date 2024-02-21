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
        // Because we create a pointer point to this obj, when we create obj we create it by smart pointer so when proc die it will
        // dealloc -> no memory leak when we use raw pointer
        // In imguilayer we get instance of this class so we need init this first in this program to dont be nullptr error
        instance = this;

        if(instance){
            ENGINE_LOG_ERROR("Only Once Instance Exit");
        }

        m_window = Window::Create();
        m_window->SetEventCallBack(std::bind_front(&Application::OnEvent, this));
        m_imGuiLayer = new ImGuiLayer();
        
        this->PushLayer(m_imGuiLayer);  // No memory leak because layerStack will free it

        m_vertexArray.reset(VertexArray::Create());
        
        float vertex[] {
            -0.5f, -0.5f, 0.0f,
             0.0f, 0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
        };

        m_vertexBuffer.reset(VertexBuffer::Create(vertex, sizeof(vertex)));
        
        BufferLayout vertexBufferLayout{
            {ShaderDataType::Float3, "a_position"}
        };

        m_vertexBuffer->SetLayout(vertexBufferLayout);

        m_vertexArray->AddVertexBuffer(m_vertexBuffer); // We need add buffer after it add layout, if not it will empty

        uint32_t index[] {
            0, 1, 2,
        };

        m_indexBuffer.reset(IndexBuffer::Create(index, 3));
        
        m_vertexArray->SetIndexBuffer(m_indexBuffer);

        std::string vertexSources {R"-(#version 460 core
        
        layout(location = 0) in vec3 a_position;

        void main(){
            gl_Position = vec4(a_position, 1.0);
        }
        
        )-"};

        std::string fragmentSources {R"-(#version 460 core
        
        out vec4 color;
        
        void main(){
            color = vec4(1.0, 0.1, 0.1, 1.0);
        }

        )-"};

        m_shader.reset(new Shader(vertexSources, fragmentSources));
    }

    void Application::Run() noexcept {

        while(m_isRunning){
            
            RenderCommand::Clear();

            RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});

            Renderer::BeginScene();

            m_shader->Bind();

            // m_vertexArray->Bind();

            Renderer::Submit(m_vertexArray);

            Renderer::EndScene();
            // glDrawElements(GL_TRIANGLES, m_indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

            for(auto&& layer : m_layerStack){
                layer->OnUpdate();
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