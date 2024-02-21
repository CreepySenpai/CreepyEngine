#pragma once

#include <memory>
#include "Window.hpp"
#include "Events/ApplicationEvent.hpp"
#include "LayerStack.hpp"
#include "Debug/ImGuiLayer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Buffer.hpp"

namespace Creepy
{
    class Application
    {
        public:
            Application() noexcept;
            virtual ~Application() = default;

            void Run() noexcept;

            void OnEvent(Event& event) noexcept;
            //

            void PushLayer(Layer* layer) noexcept;
            void PopLayer(Layer* layer) noexcept;

            void PushOverlay(Layer* overlay) noexcept;
            void PopOverlay(Layer* overlay) noexcept;

            inline Window& GetWindow() noexcept {
                return *m_window;
            }

            static inline Application& GetInstance() noexcept {
                return *instance;
            }

        private:
            bool OnWindowClose(WindowCloseEvent& event) noexcept;

            std::unique_ptr<Window> m_window;
            ImGuiLayer* m_imGuiLayer;
            bool m_isRunning{true};
            LayerStack m_layerStack;

            std::shared_ptr<Shader> m_shader;
            std::shared_ptr<VertexArray> m_vertexArray;
            std::shared_ptr<VertexBuffer> m_vertexBuffer;
            std::shared_ptr<IndexBuffer> m_indexBuffer;
        private:

            // No memory leak because point to this obj
            static Application* instance;   //singleton
    };

    std::unique_ptr<Application> CreateApplication();
}