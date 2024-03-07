#include <CreepyEngine/Controller/OrthographicCameraController.hpp>
#include <CreepyEngine/Core/Input.hpp>
#include <CreepyEngine/Core/KeyCode.hpp>

namespace Creepy {

    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) noexcept 
        : m_aspectRatio{aspectRatio}, m_rotation{rotation}
        , m_camera{-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel} {
        
        ENGINE_LOG_ERROR("Asp: {}", m_aspectRatio);
    }

    void OrthographicCameraController::OnUpdate(const TimeStep &timeStep) noexcept {

        if (Input::IsKeyPressed(CREEPY_KEY_RIGHT))
        {
            m_cameraPosition.x += m_cameraMoveSpeed * timeStep.GetSeconds();
        }
        else if (Input::IsKeyPressed(CREEPY_KEY_LEFT))
        {
            m_cameraPosition.x -= m_cameraMoveSpeed * timeStep.GetSeconds();
        }
        else if (Input::IsKeyPressed(CREEPY_KEY_UP))
        {
            m_cameraPosition.y += m_cameraMoveSpeed * timeStep.GetSeconds();
        }
        else if (Input::IsKeyPressed(CREEPY_KEY_DOWN))
        {
            m_cameraPosition.y -= m_cameraMoveSpeed * timeStep.GetSeconds();
        }

        m_camera.SetPosition(m_cameraPosition);

        if(m_rotation){

            if (Input::IsKeyPressed(CREEPY_KEY_Q))
            {
                m_cameraRotation += m_cameraRotateSpeed * timeStep.GetSeconds();
            }

            if (Input::IsKeyPressed(CREEPY_KEY_E))
            {
                m_cameraRotation -= m_cameraRotateSpeed * timeStep.GetSeconds();
            }

            m_camera.SetRotation(m_cameraRotation);
        }

        m_cameraMoveSpeed = m_zoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event &event) noexcept {
        EventDispatcher dispatcher{event};

        dispatcher.Dispatch<WindowResizeEvent>(std::bind_front(OnWindowResized, this));

        dispatcher.Dispatch<MouseScrolledEvent>(std::bind_front(OnMouseScrolled, this));
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent &event) noexcept {
        m_zoomLevel -= event.GetYOffset() * 0.25f;
        m_zoomLevel = std::max(m_zoomLevel, 0.25f);
        calculateView();
        
        ENGINE_LOG_ERROR("Zoom: {}", m_zoomLevel);
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent &event) noexcept {

        this->OnResize(static_cast<float>(event.GetWindowWidth()), static_cast<float>(event.GetWindowHeight()));

        ENGINE_LOG_ERROR("Asp: {}", m_aspectRatio);
        return false;
    }

    void OrthographicCameraController::OnResize(float width, float height) noexcept {
        
        m_aspectRatio = width / height;

        calculateView();
    }

    void OrthographicCameraController::calculateView() noexcept {

        m_bound = {.Left = -m_aspectRatio * m_zoomLevel, .Right = m_aspectRatio * m_zoomLevel, .Bottom = -m_zoomLevel, .Top = m_zoomLevel};

        m_camera.SetProjection(m_bound.Left, m_bound.Right, m_bound.Bottom, m_bound.Top);

    }
    
}