#pragma once

#include "Event.hpp"
#include <CreepyEngine/Core/MouseButtonCode.hpp>

namespace Creepy {
    class MouseMovedEvent : public Event {
        public:

            constexpr MouseMovedEvent(double x, double y) noexcept : m_mouseX{x}, m_mouseY{y} {

            }

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::MOUSE_ON_MOVED;
            };

            constexpr virtual EventType GetEventType() const noexcept override;

            constexpr virtual std::string GetEventName() const noexcept override;

            virtual std::string ToString() const noexcept override;

            constexpr virtual int GetCategoryFlags() const noexcept override;

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

            constexpr MouseScrolledEvent(double xoff, double yoff) noexcept : m_xOffset{xoff}, m_yOffset{yoff} {

            }

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::MOUSE_ON_SCROLLED;
            };

            constexpr virtual EventType GetEventType() const noexcept override;

            constexpr virtual std::string GetEventName() const noexcept override;

            virtual std::string ToString() const noexcept override;

            constexpr virtual int GetCategoryFlags() const noexcept override;

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


    class MouseButtonEvent : public Event {
        public:
            constexpr MouseButtonEvent(MouseButtonCode button) noexcept : m_button{button} {}

            constexpr inline MouseButtonCode GetButton() const noexcept {
                return m_button;
            }

            constexpr virtual int GetCategoryFlags() const noexcept override {
                return std::to_underlying(EventCategory::MOUSE_BUTTON) | std::to_underlying(EventCategory::INPUT);
            }
        protected:
            MouseButtonCode m_button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent {
        public:
            // Using Parent Constructor
            using MouseButtonEvent::MouseButtonEvent;

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::MOUSE_BUTTON_PRESSED;
            };

            constexpr virtual EventType GetEventType() const noexcept override;

            constexpr virtual std::string GetEventName() const noexcept override;

            virtual std::string ToString() const noexcept override;
        private:
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
        public:
            using MouseButtonEvent::MouseButtonEvent;

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::MOUSE_BUTTON_RELEASED;
            };

            constexpr virtual EventType GetEventType() const noexcept override;

            constexpr virtual std::string GetEventName() const noexcept override;

            virtual std::string ToString() const noexcept override;
        private:
    };
}