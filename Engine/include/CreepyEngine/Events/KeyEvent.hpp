#pragma once

#include "Event.hpp"
#include <CreepyEngine/Core/KeyCode.hpp>

namespace Creepy {
    class KeyEvent : public Event 
    {
        public:
            constexpr virtual int GetCategoryFlags() const noexcept override {
                return std::to_underlying(EventCategory::KEYBOARD) | std::to_underlying(EventCategory::INPUT);
            }
            
            constexpr inline KeyCode GetKeyCode() const noexcept {
                return m_keyCode;
            }
        protected:
            constexpr KeyEvent(KeyCode keyCode) noexcept : m_keyCode{keyCode} {

            }

            KeyCode m_keyCode;
    };

    class KeyPressedEvent : public KeyEvent {
        public:
            constexpr KeyPressedEvent(KeyCode keyCode, bool isRepeat) noexcept : KeyEvent(keyCode), m_isRepeat{isRepeat} {

            }

            // For Use With Class Name
            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::KEY_PRESSED;
            };

            // For Use With Pointer, Ref To Get Actually Event
            virtual EventType GetEventType() const noexcept override;

            virtual std::string GetEventName() const noexcept override;

            virtual std::string ToString() const noexcept override;

            constexpr inline bool IsRepeat() const {
                return m_isRepeat;
            }
        private:
            bool m_isRepeat;
    };


    class KeyReleasedEvent : public KeyEvent {
        public:
            constexpr KeyReleasedEvent(KeyCode keyCode) noexcept : KeyEvent(keyCode) {

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
            constexpr KeyTypedEvent(KeyCode keyCode) noexcept : KeyEvent(keyCode) {

            }

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::KEY_TYPED;
            };

            virtual EventType GetEventType() const noexcept override;

            virtual std::string GetEventName() const noexcept override;

            virtual std::string ToString() const noexcept override;
    };
}