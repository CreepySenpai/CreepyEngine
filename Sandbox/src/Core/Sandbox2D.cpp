#include <Core/Sandbox2D.hpp>

Sandbox2D::Sandbox2D() noexcept : Creepy::Layer{"SandBox"}, m_cameraController{600.0f / 600.0f, true}{
    Creepy::Renderer::Init();
    m_texture = Creepy::Texture2D::Create("./assets/textures/SpecularMap.png");
}

void Sandbox2D::OnAttach() noexcept
{

}

void Sandbox2D::OnDetach() noexcept
{
    
}

void Sandbox2D::OnUpdate(const Creepy::TimeStep &timeStep) noexcept
{
    m_cameraController.OnUpdate(timeStep);

    Creepy::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    Creepy::RenderCommand::Clear();

    Creepy::Renderer2D::BeginScene(m_cameraController.GetCamera());

    Creepy::Renderer2D::DrawRect({1.0f, 2.0f, 1.0f}, {5.f, 5.f}, m_playerColor);

    Creepy::Renderer2D::DrawRect({6.0f, 7.0f, 1.0f}, {10.f, 5.f}, m_texture);

    Creepy::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender() noexcept
{
    ImGui::Begin("Color Picker");
    ImGui::ColorEdit4("My Color", glm::value_ptr(m_playerColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Creepy::Event &event) noexcept
{
    m_cameraController.OnEvent(event);
}