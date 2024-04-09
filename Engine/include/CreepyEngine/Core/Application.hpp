#pragma once

#include <memory>
#include <filesystem>
#include <mutex>
#include "Window.hpp"
#include "LayerStack.hpp"
#include <CreepyEngine/Debug/ImGuiLayer.hpp>


namespace Creepy
{
    class Event;
    class WindowCloseEvent;
    class WindowResizeEvent;
    
    class Application
    {
        public:
            Application(const std::string& title = "App", uint32_t w = 600, uint32_t h = 600) noexcept;
            virtual ~Application() noexcept;

            void Run() noexcept;

            void Close() noexcept;

            void OnEvent(Event& event) noexcept;

            void PushLayer(Layer* layer) noexcept;
            void PopLayer(Layer* layer) noexcept;

            void PushOverlay(Layer* overlay) noexcept;
            void PopOverlay(Layer* overlay) noexcept;

            inline Window& GetWindow() noexcept {
                return *m_window;
            }

            inline ImGuiLayer& GetImGuiLayer() noexcept {
                return *m_imGuiLayer;
            }

            inline const std::filesystem::path& GetApplicationDirectory() const noexcept {
                return m_applicationDirectory;
            }

            void SubmitToMainThread(const std::function<void()>& func) noexcept;

            static inline Application& GetInstance() noexcept {
                return *instance;
            }

        private:
            bool OnWindowClose(WindowCloseEvent& event) noexcept;
            bool OnWindowResize(WindowResizeEvent& event) noexcept;

            void executeMainThreadQueue() noexcept;


            Scope<Window> m_window;
            ImGuiLayer* m_imGuiLayer;
            LayerStack m_layerStack;
            float m_lastFrameTime;
            bool m_isRunning{true};
            bool m_minimized{false};
            std::filesystem::path m_applicationDirectory;
            std::mutex m_mainThreadMutex;
            std::vector<std::function<void()>> m_mainThreadQueue;
        private:

            // No memory leak because point to this obj
            static Application* instance;   //singleton
    };

    Scope<Application> CreateApplication() noexcept;
}