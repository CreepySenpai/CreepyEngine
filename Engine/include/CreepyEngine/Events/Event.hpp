#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <functional>

namespace Creepy {

    enum class EventType : uint8_t {
        NONE,
        WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_ON_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVED,
        APP_TICK, APP_UPDATE, APP_RENDER,
        KEY_PRESSED, KEY_RELEASED, KEY_TYPED,
        MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_ON_MOVED, MOUSE_ON_SCROLLED
    };

    enum class EventCategory : uint8_t {
        NONE            = 0,
        APPLICATION     = 1 << 0,
        INPUT           = 1 << 1,
        KEYBOARD        = 1 << 2,
        MOUSE           = 1 << 3,
        MOUSE_BUTTON    = 1 << 4
    };

    template <typename T>
    class EventInfo{
        public:
            constexpr std::string GetEventName() noexcept {
                return static_cast<T&>(*this).GetEventName();
            }

            constexpr int GetCategoryFlags() noexcept {
                return static_cast<T&>(*this).GetCategoryFlags();
            }

            std::string ToString() noexcept {
                return static_cast<T&>(*this).ToString();
            }

            constexpr inline bool IsInCategory(EventCategory category) const noexcept {
                return GetCategoryFlags() & std::to_underlying(category);
            }
    };

    class Event
    {
        public:
            constexpr Event() noexcept = default;
            constexpr Event(EventType type, EventCategory category) noexcept : m_eventType{type}, m_eventCategory{category}{}

            constexpr EventType GetEventType() const noexcept {
                return m_eventType;
            }

            constexpr bool IsInCategory(EventCategory category) const noexcept {
                return std::to_underlying(m_eventCategory) & std::to_underlying(category);
            }

            constexpr inline bool IsHandled() const noexcept {
                return Handled;
            }

            bool Handled{false};
        
        protected:
            EventType m_eventType;
            EventCategory m_eventCategory;
    };

    class EventDispatcher {
        public:
            constexpr EventDispatcher(Event& event) noexcept : m_event{event} {

            }

            template <typename U>
            bool Dispatch(std::function<bool(U&)> func) {
                if(m_event.GetEventType() == U::GetStaticEventType()) {
                    m_event.Handled = func(*(static_cast<U*>(&m_event)));    // Cast Event To Real Type Then Call It
                    return true;
                }

                return false;
            }
        private:
            Event& m_event;
    };
}