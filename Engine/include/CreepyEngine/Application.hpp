#pragma once

#include <memory>
#include "Window.hpp"
#include "Events/ApplicationEvent.hpp"
#include "LayerStack.hpp"
#include "Debug/ImGuiLayer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/Buffer.hpp"
#include "Renderer/OrthographicCamera.hpp"
#include "Core/TimeStep.hpp"

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

            Scope<Window> m_window;
            ImGuiLayer* m_imGuiLayer;
            LayerStack m_layerStack;
            float m_lastFrameTime;
            bool m_isRunning{true};
            
        private:

            // No memory leak because point to this obj
            static Application* instance;   //singleton
    };

    Scope<Application> CreateApplication();
}