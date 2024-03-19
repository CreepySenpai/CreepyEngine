#include <CreepyEngine/Renderer/EditorCamera.hpp>
#include <CreepyEngine/Core/Input.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Creepy {

    EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip) noexcept
    : m_fov{fov}, m_aspectRatio{aspectRatio}, m_nearClip{nearClip}, m_farClip{farClip}, 
    Camera{glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)} {
        this->updateViewMatrix();
    }

    void EditorCamera::OnUpdate(TimeStep timeStep) noexcept {
        if(Input::IsKeyPressed(KeyCode::KEY_LEFT_ALT)){
            const glm::vec2& currentMousePos{Input::GetMouseX(), Input::GetMouseY()};

            glm::vec2 delta = (currentMousePos - m_previousMousePosition) * 0.003f;
            m_previousMousePosition = currentMousePos;

            if(Input::IsMouseButtonPressed(MouseButtonCode::MOUSE_LEFT)){
                this->mousePan(delta);
            }
            else if(Input::IsMouseButtonPressed(MouseButtonCode::MOUSE_RIGHT)){
                this->mouseRotate(delta);
            }
            else if(Input::IsMouseButtonPressed(MouseButtonCode::MOUSE_MIDDLE)) {
                this->mouseZoom(delta.y);
            }
        }

        this->updateViewMatrix();
    }
    void EditorCamera::OnEvent(Event& event) noexcept {
        EventDispatcher dispatcher{event};

        dispatcher.Dispatch<MouseScrolledEvent>(std::bind_front(onMouseScrolled, this));
    }

    glm::vec3 EditorCamera::GetUpDirection() const noexcept {
        return glm::rotate(GetOrientation(), glm::vec3{0.0f, 1.0f, 0.0f});
    }

    glm::vec3 EditorCamera::GetRightDirection() const noexcept {
        return glm::rotate(GetOrientation(), glm::vec3{1.0f, 0.0f, 0.0f});
    }
            
    glm::vec3 EditorCamera::GetForwardDirection() const noexcept {
        return glm::rotate(GetOrientation(), glm::vec3{0.0f, 0.0f, -1.0f});
    }

    glm::quat EditorCamera::GetOrientation() const noexcept {
        return glm::quat(glm::vec3{-m_pitch, -m_yaw, 0.0f});
    }

    void EditorCamera::updateProjectionMatrix() noexcept {
        m_aspectRatio = static_cast<float>(m_viewPortWidth) / static_cast<float>(m_viewPortHeight);
        m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
    }
    void EditorCamera::updateViewMatrix() noexcept {
        m_position = this->calculatePosition();

        m_viewMatrix = glm::translate(glm::mat4{1.0f}, m_position) * glm::toMat4(this->GetOrientation());
        m_viewMatrix = glm::inverse(m_viewMatrix);
    }

    bool EditorCamera::onMouseScrolled(MouseScrolledEvent& event) noexcept {
        float delta = event.GetYOffset() * 0.1f;
        this->mouseZoom(delta);
        this->updateViewMatrix();
        return false;
    }

    void EditorCamera::mousePan(const glm::vec2& delta) noexcept {
        auto [xSpeed, ySpeed] = this->panSpeed();

        m_focalPoint += -GetRightDirection() * delta.x * xSpeed * m_distance;
        m_focalPoint += GetUpDirection() * delta.y * ySpeed * m_distance;

    }

    void EditorCamera::mouseRotate(const glm::vec2& delta) noexcept {
        float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
        m_yaw += yawSign * delta.x + this->rotateSpeed();
        m_pitch += delta.y * this->rotateSpeed();
    }

    void EditorCamera::mouseZoom(float delta) noexcept {
        m_distance -= delta * this->zoomSpeed();

        if(m_distance < 1.0f){
            m_focalPoint += GetForwardDirection();
            m_distance = 1.0f;
        }
    }

    glm::vec3 EditorCamera::calculatePosition() noexcept {
        return m_focalPoint - GetForwardDirection() * m_distance;
    }

    std::pair<float, float> EditorCamera::panSpeed() const noexcept {
        float x = std::min( static_cast<float>(m_viewPortWidth) / 1000.0f, 2.4f);
        float xFactor = 0.0336f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(static_cast<float>(m_viewPortHeight) / 1000.f, 2.4f);
        float yFactor = 0.0336f * (y * y) - 0.1778f * y + 0.3021f;

        return {xFactor, yFactor};
    }
    float EditorCamera::zoomSpeed() const noexcept {
        float distance = m_distance * 0.2f;
        distance = std::max(distance, 0.0f);
        float speed = distance * distance;
        speed = std::min(speed, 100.0f);
        return speed;
    }

}