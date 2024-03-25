#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <CreepyEngine/Core/Core.hpp>
#include "VertexArray.hpp"

namespace Creepy
{
    class RendererAPI
    {
        public:
            virtual ~RendererAPI() noexcept = default;

            enum class API : int {
                NONE = 0, OPENGL, VULKAN, DIRECTX
            };

        public:
            
            virtual void Init() noexcept = 0;

            virtual void SetClearColor(const glm::vec4& color) noexcept = 0;
            virtual void Clear() noexcept = 0;

            virtual void EnableDepth() noexcept = 0;
            virtual void DisableDepth() noexcept = 0;

            virtual void DrawIndex(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) noexcept = 0;

            virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount = 0) noexcept = 0;

            virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept = 0;

            virtual void SetLineThickness(float thickness) noexcept = 0;

            [[nodiscard]] constexpr inline static API GetAPI() noexcept {
                return s_api;
            }
            
        private:
            static API s_api;
    };
}
