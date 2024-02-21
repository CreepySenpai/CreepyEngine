#pragma once

#include <string>
#include <memory>
#include <functional>
#include "Events/Event.hpp"

namespace Creepy {
    struct WindowProperty {
        std::string Title{"Window"};
        uint32_t Width{600}, Height{600};

        constexpr WindowProperty(const std::string& title, uint32_t w, uint32_t h) noexcept : Title{title}, Width{w}, Height{h} {

        }

        constexpr WindowProperty() noexcept = default;
    };

    class Window {
        public:

            constexpr virtual ~Window() noexcept = default;

            constexpr virtual void OnUpdate() noexcept = 0;

            constexpr virtual uint32_t GetWindowWidth() const noexcept = 0;
            constexpr virtual uint32_t GetWindowHeight() const noexcept = 0;

            constexpr virtual void SetEventCallBack(const std::function<void(Event&)>& callBack) noexcept = 0;
            constexpr virtual void SetVSync(bool setting) noexcept = 0;
            constexpr virtual bool IsVSyncEnable() const noexcept = 0;

            static std::unique_ptr<Window> Create(const WindowProperty& windowProperty = WindowProperty());

            constexpr virtual void* GetNativeWindow() const noexcept = 0;
    };
}