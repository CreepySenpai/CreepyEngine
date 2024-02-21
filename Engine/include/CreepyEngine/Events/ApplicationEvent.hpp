#pragma once

#include "Event.hpp"

namespace Creepy {

    class WindowResizeEvent : public Event {
        public:
            constexpr WindowResizeEvent(uint32_t w, uint32_t h) noexcept : m_width{w}, m_height{h} {

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

            constexpr virtual EventType GetEventType() const noexcept override;

            constexpr virtual std::string GetEventName() const noexcept override;

            virtual std::string ToString() const noexcept override;

            constexpr virtual int GetCategoryFlags() const noexcept override;

        private:
            uint32_t m_width, m_height;
    };


    class WindowCloseEvent : public Event {
        public:
            constexpr WindowCloseEvent(){}

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::WINDOW_CLOSE;
            };

            constexpr virtual EventType GetEventType() const noexcept override;

            constexpr virtual std::string GetEventName() const noexcept override;

            virtual std::string ToString() const noexcept override;

            constexpr virtual int GetCategoryFlags() const noexcept override;
        private:
    };


    class AppTickEvent : public Event {
        public:
            constexpr AppTickEvent(){}

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::APP_TICK;
            };

            constexpr virtual EventType GetEventType() const noexcept override;

            constexpr virtual std::string GetEventName() const noexcept override;

            virtual std::string ToString() const noexcept override;

            constexpr virtual int GetCategoryFlags() const noexcept override;
        private:
    };

    class AppUpdateEvent : public Event {
        public:
            constexpr AppUpdateEvent(){}

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::APP_UPDATE;
            };

            constexpr virtual EventType GetEventType() const noexcept override;

            constexpr virtual std::string GetEventName() const noexcept override;

            virtual std::string ToString() const noexcept override;

            constexpr virtual int GetCategoryFlags() const noexcept override;
        private:
    };

    class AppRenderEvent : public Event {
        public:
            constexpr AppRenderEvent(){}

            constexpr inline static EventType GetStaticEventType() noexcept {
                return EventType::APP_RENDER;
            };

            constexpr virtual EventType GetEventType() const noexcept override;

            constexpr virtual std::string GetEventName() const noexcept override;

            virtual std::string ToString() const noexcept override;

            constexpr virtual int GetCategoryFlags() const noexcept override;
        private:
    };
}