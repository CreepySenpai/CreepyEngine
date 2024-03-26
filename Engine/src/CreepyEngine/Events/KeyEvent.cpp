#include <format>
#include <CreepyEngine/Events/KeyEvent.hpp>

namespace Creepy
{
    // KeyPress
    EventType KeyPressedEvent::GetEventType() const noexcept
    {
        return GetStaticEventType();
    }

    std::string KeyPressedEvent::GetEventName() const noexcept
    {
        return "KEY_PRESSED";
    }

    std::string KeyPressedEvent::ToString() const noexcept {
        return std::format("Key Press: {}, Is Repeat: {}\n", std::to_underlying(m_keyCode), m_isRepeat);
    }

    // KeyRelease
    EventType KeyReleasedEvent::GetEventType() const noexcept
    {
        return GetStaticEventType();
    }

    std::string KeyReleasedEvent::GetEventName() const noexcept
    {
        return "KEY_RELEASED";
    }

    std::string KeyReleasedEvent::ToString() const noexcept {
        return std::format("Key Release: {}\n", std::to_underlying(m_keyCode));
    }


    EventType KeyTypedEvent::GetEventType() const noexcept {
        return GetStaticEventType();
    }

    std::string KeyTypedEvent::GetEventName() const noexcept {
        return "KEY_TYPED";
    }

    std::string KeyTypedEvent::ToString() const noexcept {
        return std::format("Key Typed: {}\n", std::to_underlying(m_keyCode));
    }
}