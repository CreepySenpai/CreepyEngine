#pragma once

#include <memory>
#include "RenderCommand.hpp"
#include "OrthographicCamera.hpp"
#include "Shader.hpp"

namespace Creepy
{

    class Renderer
    {
        public:

            static constexpr inline RendererAPI::API GetRenderAPI() noexcept {
                return RendererAPI::GetAPI();
            }
            
            static void BeginScene(OrthographicCamera& camera) noexcept;

            static void EndScene() noexcept;

            static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f)) noexcept;

        private:
            
            struct SceneData {
                glm::mat4 ViewProjectionMatrix;

            };

            static std::unique_ptr<SceneData> m_sceneData;

    };

}
