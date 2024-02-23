#pragma once

#include <memory>
#include <CreepyEngine/Core.hpp>
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

            static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f)) noexcept;

        private:
            
            struct SceneData {
                glm::mat4 ViewProjectionMatrix;

            };

            static Scope<SceneData> m_sceneData;

    };

}
