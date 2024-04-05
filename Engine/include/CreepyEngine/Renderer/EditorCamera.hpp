#pragma once

#include "Camera.hpp"
#include <glm/glm.hpp>

namespace Creepy {

    class TimeStep;
    class Event;
    class MouseScrolledEvent;


    class EditorCamera : public Camera
    {
        public:
            constexpr EditorCamera() noexcept = default;
            EditorCamera(float fov, float aspectRatio, float nearClip, float farClip) noexcept;

            void OnUpdate(TimeStep timeStep) noexcept;
            void OnEvent(Event& event) noexcept;

            constexpr float GetDistance() const noexcept {
                return m_distance;
            }

            constexpr inline void SetDistance(float distance) noexcept {
                m_distance = distance;
            }

            constexpr void SetViewPortSize(uint32_t width, uint32_t height) noexcept {
                m_viewPortWidth = width;
                m_viewPortHeight = height;
                this->updateProjectionMatrix();
            }

            const glm::mat4& GetViewMatrix() const noexcept {
                return m_viewMatrix;
            }

            glm::mat4 GetViewProjectionMatrix() const noexcept {
                return m_projectionMatrix * m_viewMatrix;
            }

            glm::vec3 GetUpDirection() const noexcept;

            glm::vec3 GetRightDirection() const noexcept;
            
            glm::vec3 GetForwardDirection() const noexcept;

            const glm::vec3& GetPosition() const noexcept {
                return m_position;
            }

            glm::quat GetOrientation() const noexcept;

            constexpr float GetPitch() const noexcept {
                return m_pitch;
            }

            constexpr float GetYaw() const noexcept {
                return m_yaw;
            }

        private:
            void updateProjectionMatrix() noexcept;
            void updateViewMatrix() noexcept;

            bool onMouseScrolled(MouseScrolledEvent& event) noexcept;

            void mousePan(const glm::vec2& delta) noexcept;
            void mouseRotate(const glm::vec2& delta) noexcept;
            void mouseZoom(float delta) noexcept;

            glm::vec3 calculatePosition() noexcept;

            std::pair<float, float> panSpeed() const noexcept;
            
            constexpr float rotateSpeed() const noexcept {
                return 0.1f;
            }
            float zoomSpeed() const noexcept;

        private:
            float m_fov{45.0f}, m_aspectRatio{1.0f}, m_nearClip{0.01f}, m_farClip{1000.f};
            float m_distance{10.0f};
            uint32_t m_viewPortWidth{1200}, m_viewPortHeight{700};
            float m_pitch{0.0f}, m_yaw{0.0f};
            glm::mat4 m_viewMatrix;
            glm::vec3 m_position{0.0f, 0.0f, 0.0f};
            glm::vec3 m_focalPoint{0.0f, 0.0f, 0.0f};
            glm::vec2 m_previousMousePosition;
    };
}