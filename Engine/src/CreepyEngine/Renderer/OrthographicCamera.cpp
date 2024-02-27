#include <CreepyEngine/Renderer/OrthographicCamera.hpp>


namespace Creepy {

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) noexcept 
    : m_projectionMatrix{glm::ortho(left, right, bottom, top)}, m_viewMatrix{1.0f} 
    {
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) noexcept {
        m_projectionMatrix = glm::ortho(left, right, bottom, top);
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;

        calculateViewMatrix();
    }
    
    void OrthographicCamera::calculateViewMatrix() noexcept {

        glm::mat4 transpose = glm::translate(glm::mat4{1.0f}, m_position);
        transpose = glm::rotate(transpose, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        // We need to reverse matrix to translate obj in order right direction
        m_viewMatrix = glm::inverse(transpose);
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;

    }

}