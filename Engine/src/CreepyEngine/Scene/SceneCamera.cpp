#include <CreepyEngine/Scene/SceneCamera.hpp>


namespace Creepy {

    SceneCamera::SceneCamera() noexcept {

        this->recalculateProjectionMatrix();

    }

    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) noexcept {
        m_projectionType = ProjectionType::ORTHOGRAPHIC;
        m_orthographicSize = size;
        m_orthographicNear = nearClip;
        m_orthographicFar = farClip;

        this->recalculateProjectionMatrix();
    }

    void SceneCamera::SetPerspective(float verticalFov, float nearClip, float farClip) noexcept {
        m_projectionType = ProjectionType::PERSPECTIVE;
        m_perspectiveFOV = verticalFov;
        m_perspectiveNear = nearClip;
        m_perspectiveFar = farClip;

        this->recalculateProjectionMatrix();
    }

    void SceneCamera::SetViewPortSize(uint32_t width, uint32_t height) noexcept {
        m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        
        this->recalculateProjectionMatrix();
    }

    void SceneCamera::recalculateProjectionMatrix() noexcept {

        if(m_projectionType == ProjectionType::ORTHOGRAPHIC){
            const float orthoLeft = -m_orthographicSize * 0.5f * m_aspectRatio;
            const float orthoRight = m_orthographicSize * 0.5f * m_aspectRatio;

            const float orthoTop = m_orthographicSize * 0.5f;
            const float orthoBottom = -m_orthographicSize * 0.5f;
            
            m_projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_orthographicNear, m_orthographicFar);
        }

        else {
            m_projectionMatrix = glm::perspective(m_perspectiveFOV, m_aspectRatio, m_perspectiveNear, m_perspectiveFar);
        }

    }

}