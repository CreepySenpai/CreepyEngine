#include <format>
#include <CreepyEngine/Events/MouseEvent.hpp>


namespace Creepy {

    constexpr EventType MouseMovedEvent::GetEventType() const noexcept
    {
        return GetStaticEventType();
    }

    constexpr std::string MouseMovedEvent::GetEventName() const noexcept
    {
        return "MOUSE_ON_MOVED";
    }

    std::string MouseMovedEvent::ToString() const noexcept
    {
        return std::format("MouseX : {}, MouseY: {}\n", m_mouseX, m_mouseY);
    }

    constexpr int MouseMovedEvent::GetCategoryFlags() const noexcept
    {
        return std::to_underlying(EventCategory::MOUSE) | std::to_underlying(EventCategory::INPUT);
    }


    // Scrolled
    

    constexpr EventType MouseScrolledEvent::GetEventType() const noexcept
    {
        return GetStaticEventType();
    }

    constexpr std::string MouseScrolledEvent::GetEventName() const noexcept
    {
        return "MOUSE_ON_SCROLLED";
    }

    std::string MouseScrolledEvent::ToString() const noexcept
    {
        return std::format("XOff: {}, YOff: {}\n", m_xOffset, m_yOffset);
    }

    constexpr int MouseScrolledEvent::GetCategoryFlags() const noexcept
    {
        return std::to_underlying(EventCategory::MOUSE) | std::to_underlying(EventCategory::INPUT);
    }

    // Mouse Press

    constexpr EventType MouseButtonPressedEvent::GetEventType() const noexcept
    {
        return GetStaticEventType();
    }

    constexpr std::string MouseButtonPressedEvent::GetEventName() const noexcept
    {
        return "MOUSE_BUTTON_PRESS";
    }

    std::string MouseButtonPressedEvent::ToString() const noexcept
    {
        return std::format("Button Press: {}", m_button);
    }

    // Mouse Released

    constexpr EventType MouseButtonReleasedEvent::GetEventType() const noexcept
    {
        return GetStaticEventType();
    }

    constexpr std::string MouseButtonReleasedEvent::GetEventName() const noexcept
    {
        return "MOUSE_BUTTON_RELEASE";
    }

    std::string MouseButtonReleasedEvent::ToString() const noexcept
    {
        return std::format("Button Release: {}", m_button);
    }
}