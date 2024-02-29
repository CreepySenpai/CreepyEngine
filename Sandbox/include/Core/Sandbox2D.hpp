#pragma once

#include <CreepyEngine.hpp>

class Sandbox2D : public Creepy::Layer
{
    public:

        Sandbox2D() noexcept;

        void OnAttach() noexcept override;

        void OnDetach() noexcept override;

        void OnUpdate(const Creepy::TimeStep& timeStep) noexcept override;

        void OnImGuiRender() noexcept override;

        void OnEvent(Creepy::Event &event) noexcept override;
    private:
        Creepy::OrthographicCameraController m_cameraController;
        Creepy::Ref<Creepy::Texture2D> m_texture;

        glm::vec4 m_playerColor{1.0f, 0.0f, 0.0f, 1.0f};
};