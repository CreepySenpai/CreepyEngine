#pragma once

#include <string>
#include <utility>
#include <functional>
#include <CreepyEngine/Core/Core.hpp>

namespace Creepy {

    enum class EventType{
        NONE,
        WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_ON_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVED,
        APP_TICK, APP_UPDATE, APP_RENDER,
        KEY_PRESSED, KEY_RELEASED, KEY_TYPED,
        MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_ON_MOVED, MOUSE_ON_SCROLLED
    };

    enum class EventCategory : int {
        NONE            = 0,
        APPLICATION     = 1 << 0,
        INPUT           = 1 << 1,
        KEYBOARD        = 1 << 2,
        MOUSE           = 1 << 3,
        MOUSE_BUTTON    = 1 << 4
    };

    class Event
    {

        public:
            constexpr virtual EventType GetEventType() const noexcept = 0;
            constexpr virtual std::string GetEventName() const noexcept = 0;
            constexpr virtual int GetCategoryFlags() const noexcept = 0;
            virtual std::string ToString() const noexcept = 0;

            constexpr inline bool IsInCategory(EventCategory category) const noexcept {
                return GetCategoryFlags() & std::to_underlying(category);
            }

            constexpr inline bool IsHandled() const noexcept {
                return Handled;
            }

            bool Handled{false};

    };
    
    class EventDispatcher {
        public:

            constexpr EventDispatcher(Event& event) noexcept : m_event{event} {

            }

            template <typename T>
            bool Dispatch(std::function<bool(T&)> func) {
                if(m_event.GetEventType() == T::GetStaticEventType()) {
                    m_event.Handled = func(*(dynamic_cast<T*>(&m_event)));    // Cast Event To Real Type Then Call It
                    return true;
                }

                return false;
            }
        private:
            Event& m_event;
    };

    inline std::ostream& operator<< (std::ostream& stream, const Event& event) {
        stream << event.ToString();
        return stream;
    }
}