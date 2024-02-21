#pragma once

#include "RenderCommand.hpp"

namespace Creepy
{

    class Renderer
    {
        public:

            static constexpr inline RendererAPI::API GetRenderAPI() noexcept {
                return RendererAPI::GetAPI();
            }
            
            static void BeginScene() noexcept;

            static void EndScene() noexcept;

            static void Submit(const std::shared_ptr<VertexArray>& vertexArray) noexcept;

        private:
            
    };

}
