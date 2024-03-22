#pragma once

#include <CreepyEngine/Renderer/Camera.hpp>
#include <cstdint>

namespace Creepy {
    
    class SceneCamera : public Camera
    {
        public:

            enum class ProjectionType : uint32_t {
                ORTHOGRAPHIC = 0, PERSPECTIVE
            };

        public:
            SceneCamera() noexcept;

            void SetOrthographic(float size, float nearClip, float farClip) noexcept;

            void SetPerspective(float verticalFov, float nearClip, float farClip) noexcept;

            void SetViewPortSize(uint32_t width, uint32_t height) noexcept;

            [[nodiscard]] constexpr inline ProjectionType GetProjectionType() const noexcept {
                return m_projectionType;
            }

            void SetProjectionType(ProjectionType newType) noexcept {
                m_projectionType = newType;
                this->recalculateProjectionMatrix();
            }

            [[nodiscard]] constexpr inline float GetOrthographicSize() const noexcept {
                return m_orthographicSize;
            }

            void SetOrthographicSize(float size) noexcept {
                m_orthographicSize = size;
                this->recalculateProjectionMatrix();
            }

            constexpr inline float GetOrthographicNearClip() const noexcept {
                return m_orthographicNear;
            }

            [[nodiscard]] constexpr inline float GetOrthographicFarClip() const noexcept {
                return m_orthographicFar;
            }

            void SetOrthographicNearClip(float nearClip) noexcept {
                m_orthographicNear = nearClip;
                this->recalculateProjectionMatrix();
            }

            void SetOrthographicFarClip(float farClip) noexcept {
                m_orthographicFar = farClip;
                this->recalculateProjectionMatrix();
            }

            /*=============================================================*/

            [[nodiscard]] constexpr inline float GetPerspectiveVerticalFOV() const noexcept {
                return glm::degrees(m_perspectiveFOV);
            }

            void SetPerspectiveVerticalFOV(float verticalFov) noexcept {
                m_perspectiveFOV = glm::radians(verticalFov);
                this->recalculateProjectionMatrix();
            }

            constexpr inline float GetPerspectiveNearClip() const noexcept {
                return m_perspectiveNear;
            }

            [[nodiscard]] constexpr inline float GetPerspectiveFarClip() const noexcept {
                return m_perspectiveFar;
            }

            void SetPerspectiveNearClip(float nearClip) noexcept {
                m_perspectiveNear = nearClip;
                this->recalculateProjectionMatrix();
            }

            void SetPerspectiveFarClip(float farClip) noexcept {
                m_perspectiveFar = farClip;
                this->recalculateProjectionMatrix();
            }

        private:
            void recalculateProjectionMatrix() noexcept;

        private:
            ProjectionType m_projectionType{ProjectionType::ORTHOGRAPHIC};
            float m_orthographicSize{10.0f};
            float m_orthographicNear{-1.0f};
            float m_orthographicFar{1.0f};

            float m_perspectiveFOV{glm::radians(45.0f)};
            float m_perspectiveNear{0.001f};
            float m_perspectiveFar{1000.0f};

            float m_aspectRatio{1.0f};
    };
    
   
}