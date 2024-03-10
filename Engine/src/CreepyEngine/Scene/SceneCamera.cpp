#include <CreepyEngine/Scene/SceneCamera.hpp>


namespace Creepy {

    SceneCamera::SceneCamera() noexcept {

        this->recalculateProjectionMatrix();

    }

    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) noexcept {
        m_orthographicSize = size;
        m_orthographicNear = nearClip;
        m_orthographicFar = farClip;

        this->recalculateProjectionMatrix();
    }

    void SceneCamera::SetViewPortSize(uint32_t width, uint32_t height) noexcept {
        m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);

        this->recalculateProjectionMatrix();
    }

    void SceneCamera::recalculateProjectionMatrix() noexcept {

        const float orthoLeft = -m_orthographicSize * 0.5f * m_aspectRatio;
        const float orthoRight = m_orthographicSize * 0.5f * m_aspectRatio;

        const float orthoTop = m_orthographicSize * 0.5f;
        const float orthoBottom = -m_orthographicSize * 0.5f;
        
        m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_orthographicNear, m_orthographicFar);

    }

}