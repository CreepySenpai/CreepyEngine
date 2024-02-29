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
        Creepy::Ref<Creepy::Shader> m_shader;
        Creepy::Ref<Creepy::VertexArray> m_vertexArray;
        Creepy::Ref<Creepy::VertexBuffer> m_vertexBuffer;
        Creepy::Ref<Creepy::IndexBuffer> m_indexBuffer;
        Creepy::OrthographicCameraController m_cameraController;

        glm::vec4 m_playerColor{1.0f, 0.0f, 0.0f, 1.0f};
};