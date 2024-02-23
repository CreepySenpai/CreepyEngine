#pragma once

#define GLFW_DLL
#include <memory>
#include <GLFW/glfw3.h>
#include <CreepyEngine/Window.hpp>
#include <CreepyEngine/Renderer/GraphicContext.hpp>
#include <CreepyEngine/Core.hpp>

namespace Creepy {
    class WindowsWindow : public Window {
        public:
            WindowsWindow(const WindowProperty& windowProperty) noexcept;

            // constexpr virtual ~WindowsWindow() = default;

            constexpr virtual void OnUpdate() noexcept override;

            constexpr virtual uint32_t GetWindowWidth() const noexcept override {
                return m_windowData.Width;
            }
            constexpr virtual uint32_t GetWindowHeight() const noexcept override {
                return m_windowData.Height;
            }

            constexpr virtual void SetEventCallBack(const std::function<void(Event&)>& callBack) noexcept override;
            constexpr virtual void SetVSync(bool setting) noexcept override;
            constexpr virtual bool IsVSyncEnable() const noexcept override;

            constexpr inline virtual void* GetNativeWindow() const noexcept override {
                return reinterpret_cast<void*>(m_window.get());
            }

        private:
            constexpr virtual void Init(const WindowProperty& windowProperty) noexcept;
            constexpr void AttachEvent() noexcept;
            constexpr virtual void ShutDown() noexcept;

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