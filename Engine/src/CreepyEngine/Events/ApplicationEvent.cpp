#include <CreepyEngine/Events/ApplicationEvent.hpp>


namespace Creepy {

    // Resize
    constexpr EventType WindowResizeEvent::GetEventType() const noexcept
    {
        return GetStaticEventType();
    }

    constexpr std::string WindowResizeEvent::GetEventName() const noexcept
    {
        return "WINDOW_RESIZE";
    }

    std::string WindowResizeEvent::ToString() const noexcept
    {
        return std::format("Width : {}, Height: {}\n", m_width, m_height);
    }

    constexpr int WindowResizeEvent::GetCategoryFlags() const noexcept
    {
        return std::to_underlying(EventCategory::APPLICATION);
    }


    // Close
    constexpr EventType WindowCloseEvent::GetEventType() const noexcept
    {
        return GetStaticEventType();
    }

    constexpr std::string WindowCloseEvent::GetEventName() const noexcept
    {
        return "WINDOW_CLOSE";
    }

    std::string WindowCloseEvent::ToString() const noexcept
    {
        return std::format("Window Close\n");
    }

    constexpr int WindowCloseEvent::GetCategoryFlags() const noexcept
    {
        return std::to_underlying(EventCategory::APPLICATION);
    }

    // App Tick

    constexpr EventType AppTickEvent::GetEventType() const noexcept
    {
        return GetStaticEventType();
    }

    constexpr std::string AppTickEvent::GetEventName() const noexcept
    {
        return "APP_TICK";
    }

    std::string AppTickEvent::ToString() const noexcept
    {
        return std::format("App Tick\n");
    }

    constexpr int AppTickEvent::GetCategoryFlags() const noexcept
    {
        return std::to_underlying(EventCategory::APPLICATION);
    }

    // Update

    constexpr EventType AppUpdateEvent::GetEventType() const noexcept
    {
        return GetStaticEventType();
    }

    constexpr std::string AppUpdateEvent::GetEventName() const noexcept
    {
        return "APP_UPDATE";
    }

    std::string AppUpdateEvent::ToString() const noexcept
    {
        return std::format("App Update\n");
    }

    constexpr int AppUpdateEvent::GetCategoryFlags() const noexcept
    {
        return std::to_underlying(EventCategory::APPLICATION);
    }

    // Render

    constexpr EventType AppRenderEvent::GetEventType() const noexcept
    {
        return GetStaticEventType();
    }

    constexpr std::string AppRenderEvent::GetEventName() const noexcept
    {
        return "APP_UPDATE";
    }

    std::string AppRenderEvent::ToString() const noexcept
    {
        return std::format("App Update\n");
    }

    constexpr int AppRenderEvent::GetCategoryFlags() const noexcept
    {
        return std::to_underlying(EventCategory::APPLICATION);
    }
}