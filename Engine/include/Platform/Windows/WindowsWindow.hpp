#pragma once

#define GLFW_DLL
#include <memory>
#include <GLFW/glfw3.h>
#include <CreepyEngine/Core/Window.hpp>

namespace Creepy {

    class GraphicContext;

    class WindowsWindow : public Window {
        public:
            WindowsWindow(const WindowProperty& windowProperty) noexcept;

            constexpr virtual void OnUpdate() noexcept override;

            void ShutDown() noexcept override;

            [[nodiscard]] constexpr uint32_t GetWindowWidth() const noexcept override {
                return m_windowData.Width;
            }

            [[nodiscard]] constexpr uint32_t GetWindowHeight() const noexcept override {
                return m_windowData.Height;
            }

            [[nodiscard]] Ref<GraphicContext> GetGraphicContext() noexcept override {
                return m_context;
            }

            constexpr void SetEventCallBack(const std::function<void(Event&)>& callBack) noexcept override;
            constexpr void SetVSync(bool setting) noexcept override;
            constexpr bool IsVSyncEnable() const noexcept override;

            [[nodiscard]] constexpr inline void* GetNativeWindow() const noexcept override {
                return static_cast<void*>(m_window.get());
            }

        private:
            constexpr void Init(const WindowProperty& windowProperty) noexcept;
            constexpr void AttachEvent() noexcept;

            Ref<GLFWwindow> m_window;

            struct WindowData {
                std::string Title{};
                uint32_t Width{}, Height{};
                bool VSync{};
                std::function<void(Event&)> EventCallBack;
            };

            WindowData m_windowData;

            Ref<GraphicContext> m_context;
    };
}