#include <CreepyEngine/Renderer/Renderer.hpp>

namespace Creepy
{

    Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData();

    void Renderer::BeginScene(OrthographicCamera& camera) noexcept {
        m_sceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

    }

    void Renderer::EndScene() noexcept {

    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray) noexcept {
        shader->Bind();
        shader->SetUniformMat4("u_viewProjectionMatrix", m_sceneData->ViewProjectionMatrix);
        vertexArray->Bind();
        RenderCommand::DrawIndex(vertexArray);
    }
}
