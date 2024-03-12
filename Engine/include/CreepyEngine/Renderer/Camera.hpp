#pragma once

#include <glm/glm.hpp>

namespace Creepy {

    class Camera {

        public:
            constexpr Camera() noexcept = default;
            constexpr Camera(const glm::mat4& projection) noexcept : m_projectionMatrix{projection} {}

            constexpr inline const glm::mat4& GetProjection() const noexcept {
                return m_projectionMatrix;
            }

        protected:
            glm::mat4 m_projectionMatrix{0.0f};
    };

}