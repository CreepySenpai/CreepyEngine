#include <CreepyEngine/Renderer/Renderer.hpp>

namespace Creepy
{

    void Renderer::BeginScene() noexcept {

    }

    void Renderer::EndScene() noexcept {

    }

    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) noexcept {
        vertexArray->Bind();
        RenderCommand::DrawIndex(vertexArray);
    }
}
