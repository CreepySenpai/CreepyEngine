#pragma once

#include <CreepyEngine/Renderer/RendererAPI.hpp>

namespace Creepy
{
    class OpenGLRendererAPI : public RendererAPI
    {
        public:
            OpenGLRendererAPI() noexcept {}

            void Init() noexcept override;

            void SetClearColor(const glm::vec4& color) noexcept override;

            void Clear() noexcept override;

            void DrawIndex(const Ref<VertexArray>& vertexArray) noexcept override;

        private:
            
    }; 
}