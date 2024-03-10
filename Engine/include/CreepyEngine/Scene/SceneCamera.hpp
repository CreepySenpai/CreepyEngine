#pragma once

#include <CreepyEngine/Renderer/Camera.hpp>

namespace Creepy {
    
    class SceneCamera : public Camera
    {
        public:
            SceneCamera() noexcept;

            void SetOrthographic(float size, float nearClip, float farClip) noexcept;

            void SetViewPortSize(uint32_t width, uint32_t height) noexcept;

            constexpr inline float GetOrthographicSize() const noexcept {
                return m_orthographicSize;
            }

            void SetOrthographicSize(float size) noexcept {
                m_orthographicSize = size;
                recalculateProjectionMatrix();
            }

        private:
            void recalculateProjectionMatrix() noexcept;

        private:
            float m_orthographicSize{10.0f};
            float m_orthographicNear{-1.0f};
            float m_orthographicFar{1.0f};
            float m_aspectRatio{1.0f};
    };
    
   
}