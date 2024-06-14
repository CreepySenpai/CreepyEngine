#pragma once

#include <string>
#include <cstdint>
#include <utility>
#include <format>
#include "Event.hpp"

namespace Creepy {

    class WindowResizeEvent : public Event
    {
        public:
            constexpr WindowResizeEvent(uint32_t w, uint32_t h) noexcept : Event{EventType::WINDOW_RESIZE, EventCategory::APPLICATION}, m_width{w}, m_height{h} {

            }

            constexpr inline uint32_t GetWindowWidth() const noexcept {
                return m_width;
            }

            constexpr inline uint32_t GetWindowHeight() const noexcept {
                return m_height;
            }

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::WINDOW_RESIZE;
            };

            constexpr std::string GetEventName() const noexcept {
                return "WINDOW_RESIZE";
            }

            constexpr int GetCategoryFlags() const noexcept {
                return std::to_underlying(m_eventCategory);
            }

            std::string ToString() const noexcept {
                return std::format("Width : {}, Height: {}\n", m_width, m_height);
            }

        private:
            uint32_t m_width, m_height;
    };

    class WindowCloseEvent : public Event {
        public:
            constexpr WindowCloseEvent() noexcept : Event{EventType::WINDOW_CLOSE, EventCategory::APPLICATION} {}

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::WINDOW_CLOSE;
            };

            constexpr std::string GetEventName() const noexcept {
                return "WINDOW_CLOSE";
            }

            constexpr int GetCategoryFlags() const noexcept {
                return std::to_underlying(m_eventCategory);
            }

            std::string ToString() const noexcept {
                return "Window Close";
            }
    };

    class AppTickEvent : public Event {
        public:
            constexpr AppTickEvent() noexcept : Event{EventType::APP_TICK, EventCategory::APPLICATION} {}

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::APP_TICK;
            };

            constexpr std::string GetEventName() const noexcept {
                return "APP_TICK";
            }

            constexpr int GetCategoryFlags() const noexcept {
                return std::to_underlying(m_eventCategory);
            }

            std::string ToString() const noexcept {
                return "App Tick";
            }
    };

    class AppUpdateEvent : public Event {
        public:
            constexpr AppUpdateEvent() noexcept : Event{EventType::APP_UPDATE, EventCategory::APPLICATION} {}

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::APP_UPDATE;
            };

            constexpr std::string GetEventName() const noexcept {
                return "APP_UPDATE";
            }

            constexpr int GetCategoryFlags() const noexcept {
                return std::to_underlying(m_eventCategory);
            }

            std::string ToString() const noexcept {
                return "App Update";
            }
    };

    class AppRenderEvent : public Event {
        public:
            constexpr AppRenderEvent() noexcept : Event{EventType::APP_RENDER, EventCategory::APPLICATION} {}

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::APP_RENDER;
            };

            constexpr std::string GetEventName() const noexcept {
                return "APP_RENDER";
            }

            constexpr int GetCategoryFlags() const noexcept {
                return std::to_underlying(m_eventCategory);
            }

            std::string ToString() const noexcept {
                return "App Render";
            }
    };
}