#include <CreepyEngine/Renderer/Renderer.hpp>
#include <Platform/OpenGL/OpenGLShader.hpp>
namespace Creepy
{

    Scope<Renderer::SceneData> Renderer::m_sceneData = std::make_unique<Renderer::SceneData>();

    void Renderer::BeginScene(OrthographicCamera& camera) noexcept {
        m_sceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

    }

    void Renderer::EndScene() noexcept {

    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) noexcept {
        shader->Bind();

        std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("u_viewProjectionMatrix", m_sceneData->ViewProjectionMatrix);
        // Transform per obj
        std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("u_transformMatrix", transform);
        vertexArray->Bind();
        RenderCommand::DrawIndex(vertexArray);
    }
}
