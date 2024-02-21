#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "VertexArray.hpp"

namespace Creepy
{
    class RendererAPI
    {
        public:
            virtual ~RendererAPI() = default;

            enum class API : int {
                NONE = 0, OPENGL, VULKAN, DIRECTX
            };

        public:
            virtual void SetClearColor(const glm::vec4& color) noexcept = 0;
            virtual void Clear() noexcept = 0;

            virtual void DrawIndex(const std::shared_ptr<VertexArray>& vertexArray) noexcept = 0;

            constexpr inline static API GetAPI() noexcept {
                return s_api;
            }
            
        private:
            static API s_api;
    };
}
