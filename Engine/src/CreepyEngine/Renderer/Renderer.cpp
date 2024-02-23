#include <CreepyEngine/Renderer/Renderer.hpp>

namespace Creepy
{

    std::unique_ptr<Renderer::SceneData> Renderer::m_sceneData = std::make_unique<Renderer::SceneData>();

    void Renderer::BeginScene(OrthographicCamera& camera) noexcept {
        m_sceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

    }

    void Renderer::EndScene() noexcept {

    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform) noexcept {
        shader->Bind();
        shader->SetUniformMat4("u_viewProjectionMatrix", m_sceneData->ViewProjectionMatrix);
    
        // Transform per obj
        shader->SetUniformMat4("u_transformMatrix", transform);
        vertexArray->Bind();
        RenderCommand::DrawIndex(vertexArray);
    }
}
