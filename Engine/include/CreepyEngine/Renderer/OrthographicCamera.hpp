#pragma once

#include <glm/glm.hpp>

namespace Creepy {

    class OrthographicCamera
    {
        public:
            OrthographicCamera(float left, float right, float bottom, float top) noexcept;

            constexpr inline void SetPosition(const glm::vec3& pos) noexcept {
                m_position = pos;

                calculateViewMatrix();
            }

            constexpr inline const glm::vec3& GetPosition() const noexcept {
                return m_position;
            }

            constexpr inline void SetRotation(float rot) noexcept {
                m_rotation = rot;

                calculateViewMatrix();
            }

            constexpr inline float GetRotation() const noexcept {
                return m_rotation;
            }

            constexpr inline const glm::mat4& GetProjectionMatrix() const noexcept {
                return m_projectionMatrix;
            }

            constexpr inline const glm::mat4& GetViewMatrix() const noexcept {
                return m_viewMatrix;
            }

            constexpr inline const glm::mat4& GetViewProjectionMatrix() const noexcept {
                return m_viewProjectionMatrix;
            }

        private:
            // After we change value of position and rotation we need to recalculate view matrix
            void calculateViewMatrix() noexcept;

        private:
            glm::mat4 m_projectionMatrix;
            glm::mat4 m_viewMatrix;
            glm::mat4 m_viewProjectionMatrix;

            glm::vec3 m_position{0.0f};
            float m_rotation{0.0f};
    };

}