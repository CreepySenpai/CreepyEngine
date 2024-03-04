#pragma once

#include <CreepyEngine/Renderer/RendererAPI.hpp>

namespace Creepy
{
    class OpenGLRendererAPI : public RendererAPI
    {
        public:
            // OpenGLRendererAPI() noexcept = default;

            void Init() noexcept override;

            void SetClearColor(const glm::vec4& color) noexcept override;

            void Clear() noexcept override;

            void DrawIndex(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) noexcept override;

            void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept override;

        private:
            
    }; 
}