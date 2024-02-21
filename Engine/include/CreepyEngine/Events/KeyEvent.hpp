#pragma once

#include "Event.hpp"

namespace Creepy {
    class KeyEvent : public Event 
    {
        public:
            constexpr virtual int GetCategoryFlags() const noexcept override {
                return std::to_underlying(EventCategory::KEYBOARD) | std::to_underlying(EventCategory::INPUT);
            }
            
            constexpr inline int GetKeyCode() const noexcept {
                return m_keyCode;
            }
        protected:
            constexpr KeyEvent(int keyCode) noexcept : m_keyCode{keyCode} {

            }

            int m_keyCode{0};
    };

    class KeyPressedEvent : public KeyEvent {
        public:
            constexpr KeyPressedEvent(int keyCode, int repeatCount) noexcept : KeyEvent(keyCode), m_repeatCount{repeatCount} {

            }

            // For Use With Class Name
            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::KEY_PRESSED;
            };

            // For Use With Pointer, Ref To Get Actually Event
            virtual EventType GetEventType() const noexcept override;

            virtual std::string GetEventName() const noexcept override;

            virtual std::string ToString() const noexcept override;

            constexpr inline int GetRepeatCount() const {
                return m_repeatCount;
            }
        private:
            int m_repeatCount{0};
    };


    class KeyReleasedEvent : public KeyEvent {
        public:
            constexpr KeyReleasedEvent(int keyCode) noexcept : KeyEvent(keyCode) {

            }

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::KEY_RELEASED;
            };

            virtual EventType GetEventType() const noexcept override;

            virtual std::string GetEventName() const noexcept override;

            virtual std::string ToString() const noexcept override;
        private:

    };

    class KeyTypedEvent : public KeyEvent {
        public:
            constexpr KeyTypedEvent(int keyCode) noexcept : KeyEvent(keyCode) {

            }

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::KEY_TYPED;
            };

            virtual EventType GetEventType() const noexcept override;

            virtual std::string GetEventName() const noexcept override;

            virtual std::string ToString() const noexcept override;
    };
}