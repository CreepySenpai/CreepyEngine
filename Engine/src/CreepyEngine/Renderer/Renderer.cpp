#include <CreepyEngine/Renderer/Renderer.hpp>
#include <Platform/OpenGL/OpenGLShader.hpp>
#include <CreepyEngine/Renderer/Renderer2D.hpp>
namespace Creepy
{

    Scope<Renderer::SceneData> Renderer::m_sceneData = std::make_unique<Renderer::SceneData>();

    void Renderer::Init() noexcept {
        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::ShutDown() noexcept {
        Renderer2D::ShutDown();
    }

    void Renderer::BeginScene(const Camera& camera) noexcept {
        
    }

    void Renderer::BeginScene(const OrthographicCamera& camera) noexcept {
        m_sceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

    }

    void Renderer::EndScene() noexcept {

    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) noexcept {
        // shader->Bind();

        // shader->SetUniformMat4("u_viewProjectionMatrix", m_sceneData->ViewProjectionMatrix);
        // Transform per obj
        // shader->SetUniformMat4("u_transformMatrix", transform);
        // vertexArray->Bind();
        // RenderCommand::DrawIndex(vertexArray);
    }

    void Renderer::WindowResize(uint32_t width, uint32_t height) noexcept {
        RenderCommand::SetViewPort(0, 0, width, height);
    }
}
