#pragma once

#include <format>

#include "Event.hpp"
#include <CreepyEngine/Core/KeyCode.hpp>

namespace Creepy {

    class KeyPressedEvent : public Event {
        public:
            constexpr KeyPressedEvent(KeyCode keyCode) noexcept : Event{EventType::KEY_PRESSED, EventCategory::KEYBOARD}, m_keyCode{keyCode} {

            }
            constexpr KeyPressedEvent(KeyCode keyCode, bool isRepeat) noexcept : Event{EventType::KEY_PRESSED, EventCategory::KEYBOARD}, m_keyCode(keyCode), m_isRepeat{isRepeat} {

            }

            constexpr inline KeyCode GetKeyCode() const noexcept {
                return m_keyCode;
            }

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::KEY_PRESSED;
            };

            constexpr std::string GetEventName() const noexcept {
                return "KEY_PRESSED";
            }

            constexpr int GetCategoryFlags() const noexcept {
                // return std::to_underlying(EventType::KEY_PRESSED, ::KEYBOARD) | std::to_underlying(EventType::KEY_PRESSED, ::INPUT);
                return std::to_underlying(m_eventCategory);
            }

            std::string ToString() const noexcept {
                return std::format("Key Press: {}, Is Repeat: {}\n", std::to_underlying(m_keyCode), m_isRepeat);
            }

            constexpr inline bool IsRepeat() const {
                return m_isRepeat;
            }
        private:
            KeyCode m_keyCode;
            bool m_isRepeat;
    };

    class KeyReleasedEvent : public Event {
        public:
            constexpr KeyReleasedEvent(KeyCode keyCode) noexcept : Event{EventType::KEY_RELEASED, EventCategory::KEYBOARD}, m_keyCode(keyCode) {

            }

            constexpr inline KeyCode GetKeyCode() const noexcept {
                return m_keyCode;
            }

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::KEY_RELEASED;
            };

            constexpr std::string GetEventName() const noexcept {
                return "KEY_RELEASED";
            }

            constexpr int GetCategoryFlags() const noexcept {
                // return std::to_underlying(EventType::KEY_PRESSED, ::KEYBOARD) | std::to_underlying(EventType::KEY_PRESSED, ::INPUT);
                return std::to_underlying(m_eventCategory);
            }

            std::string ToString() const noexcept {
                return std::format("Key Release: {}\n", std::to_underlying(m_keyCode));
            }
        private:
            KeyCode m_keyCode;
    };

    class KeyTypedEvent : public Event {
        public:
            constexpr KeyTypedEvent(KeyCode keyCode) noexcept : Event{EventType::KEY_TYPED, EventCategory::KEYBOARD}, m_keyCode(keyCode) {

            }

            constexpr inline KeyCode GetKeyCode() const noexcept {
                return m_keyCode;
            }

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::KEY_TYPED;
            };

            constexpr std::string GetEventName() const noexcept {
                return "KEY_TYPED";
            }

            constexpr int GetCategoryFlags() const noexcept {
                // return std::to_underlying(EventType::KEY_PRESSED, ::KEYBOARD) | std::to_underlying(EventType::KEY_PRESSED, ::INPUT);
                return std::to_underlying(m_eventCategory);
            }

            std::string ToString() const noexcept {
                return std::format("Key Typed: {}\n", std::to_underlying(m_keyCode));
            }
        private:
            KeyCode m_keyCode;
    };

}