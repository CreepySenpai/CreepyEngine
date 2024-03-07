#pragma once

#include <CreepyEngine/Renderer/OrthographicCamera.hpp>
#include <CreepyEngine/Core/TimeStep.hpp>
#include <CreepyEngine/Events/ApplicationEvent.hpp>
#include <CreepyEngine/Events/MouseEvent.hpp>
#include <glm/glm.hpp>

namespace Creepy
{
    class OrthographicCameraController
    {
        public:
            OrthographicCameraController(float aspectRatio, bool rotation = false) noexcept;
            
            void OnUpdate(const TimeStep& timeStep) noexcept;
            void OnEvent(Event& event) noexcept;

            constexpr const inline OrthographicCamera& GetCamera() const noexcept {
                return m_camera;
            }

            constexpr inline OrthographicCamera& GetCamera() noexcept {
                return m_camera;
            }

            constexpr inline void SetZoomLevel(float zoomLevel) noexcept {
                m_zoomLevel = zoomLevel;
                calculateView();
            }

            constexpr inline float GetZoomLevel() const {
                return m_zoomLevel;
            }

            void OnResize(float width, float height) noexcept;

        private:
            bool OnMouseScrolled(MouseScrolledEvent& event) noexcept;
            bool OnWindowResized(WindowResizeEvent& event) noexcept;

            void calculateView() noexcept;

        private:
            float m_aspectRatio;
            float m_zoomLevel{1.0f};
            OrthographicCamera m_camera;
            glm::vec3 m_cameraPosition{0.0f, 0.0f, 0.0f};
            float m_cameraRotation{0.0f};
            float m_cameraMoveSpeed{1.0f};
            float m_cameraRotateSpeed{10.0f};
            bool m_rotation;

            struct ScreenBound{
                float Left, Right, Bottom, Top;
            };
            ScreenBound m_bound{};

    };
    
    
}
