#pragma once

#include <memory>
#include <CreepyEngine/Core/Core.hpp>
#include "RenderCommand.hpp"
#include "Camera.hpp"
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

            static void Init() noexcept;

            static void ShutDown() noexcept;
            
            static void BeginScene(const Camera& camera) noexcept;

            static void BeginScene(const OrthographicCamera& camera) noexcept;  // TODO: Remove

            static void EndScene() noexcept;

            static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f)) noexcept;

            static void WindowResize(uint32_t width, uint32_t height) noexcept;
        private:
            
            struct SceneData {
                glm::mat4 ViewProjectionMatrix;

            };

            static Scope<SceneData> m_sceneData;

    };

}
