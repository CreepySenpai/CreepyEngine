#pragma once

#include <format>

#include "Event.hpp"
#include <CreepyEngine/Core/MouseButtonCode.hpp>

namespace Creepy {
    
    class MouseMovedEvent : public Event {

        public:
            constexpr MouseMovedEvent(double x, double y) noexcept : Event{EventType::MOUSE_ON_MOVED, EventCategory::MOUSE}, m_mouseX{x}, m_mouseY{y} {

            }

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::MOUSE_ON_MOVED;
            };

            constexpr std::string GetEventName() const noexcept {
                return "MOUSE_ON_MOVED";
            }

            constexpr int GetCategoryFlags() const noexcept {
                // return std::to_underlying(EventCategory::MOUSE) | std::to_underlying(EventCategory::INPUT);
                return std::to_underlying(m_eventCategory);
            }

            std::string ToString() const noexcept {
                return std::format("MouseX : {}, MouseY: {}\n", m_mouseX, m_mouseY);
            }

            constexpr inline double GetMouseX() const noexcept {
                return m_mouseX;
            }

            constexpr inline double GetMouseY() const noexcept {
                return m_mouseY;
            }

        private:
            double m_mouseX;
            double m_mouseY;
    };


    class MouseScrolledEvent : public Event {
        public:

            constexpr MouseScrolledEvent(double xoff, double yoff) noexcept : Event{EventType::MOUSE_ON_SCROLLED, EventCategory::MOUSE}, m_xOffset{xoff}, m_yOffset{yoff} {

            }

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::MOUSE_ON_SCROLLED;
            };

            constexpr std::string GetEventName() const noexcept {
                return "MOUSE_ON_SCROLLED";
            }

            constexpr int GetCategoryFlags() const noexcept {
                // return std::to_underlying(EventCategory::MOUSE) | std::to_underlying(EventCategory::INPUT);
                return std::to_underlying(m_eventCategory);
            }

            std::string ToString() const noexcept {
                return std::format("XOff: {}, YOff: {}\n", m_xOffset, m_yOffset);
            }

            constexpr inline double GetXOffset() const noexcept {
                return m_xOffset;
            }

            constexpr inline double GetYOffset() const noexcept {
                return m_yOffset;
            }

        private:
            double m_xOffset;
            double m_yOffset;
    };

    class MouseButtonPressedEvent : public Event {
        public:
            constexpr MouseButtonPressedEvent(MouseButtonCode button) noexcept : Event{EventType::MOUSE_BUTTON_PRESSED, EventCategory::MOUSE_BUTTON}, m_button{button} {}

            constexpr inline MouseButtonCode GetButton() const noexcept {
                return m_button;
            }

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::MOUSE_BUTTON_PRESSED;
            };

            constexpr int GetCategoryFlags() const noexcept {
                // return std::to_underlying(EventCategory::MOUSE_BUTTON) | std::to_underlying(EventCategory::INPUT);
                return std::to_underlying(m_eventCategory);
            }

            constexpr std::string GetEventName() const noexcept {
                return "MOUSE_BUTTON_PRESS";
            }

            std::string ToString() const noexcept {
                return std::format("Button Press: {}", std::to_underlying(m_button));
            }
        private:
            MouseButtonCode m_button;
    };

    class MouseButtonReleasedEvent : public Event {
        public:
            constexpr MouseButtonReleasedEvent(MouseButtonCode button) noexcept : Event{EventType::MOUSE_BUTTON_RELEASED, EventCategory::MOUSE_BUTTON}, m_button{button} {}

            constexpr inline MouseButtonCode GetButton() const noexcept {
                return m_button;
            }

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::MOUSE_BUTTON_RELEASED;
            };

            constexpr int GetCategoryFlags() const noexcept {
                // return std::to_underlying(EventCategory::MOUSE_BUTTON) | std::to_underlying(EventCategory::INPUT);
                return std::to_underlying(m_eventCategory);
            }

            constexpr std::string GetEventName() const noexcept {
                return "MOUSE_BUTTON_RELEASE";
            }

            std::string ToString() const noexcept {
                return std::format("Button Release: {}", std::to_underlying(m_button));
            }
            
        private:
            MouseButtonCode m_button;
    };
}