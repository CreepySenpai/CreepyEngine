#pragma once
#include <memory>
#include <CreepyEngine/Core/Core.hpp>
#include "RendererAPI.hpp"

namespace Creepy
{
    class RenderCommand
    {
        public:

            constexpr static inline void Init() noexcept {
                s_rendererAPI->Init();
            }
            
            constexpr static inline void DrawIndex(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) noexcept {
                s_rendererAPI->DrawIndex(vertexArray, indexCount);
            }

            constexpr static inline void SetClearColor(const glm::vec4& color) noexcept {
                s_rendererAPI->SetClearColor(color);
            }

            constexpr static inline void Clear() noexcept {
                s_rendererAPI->Clear();
            }

            constexpr static inline void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept {
                s_rendererAPI->SetViewPort(x, y, width, height);
            }

            constexpr static inline void EnableDepth() noexcept {
                s_rendererAPI->EnableDepth();
            }

            constexpr static inline void DisableDepth() noexcept {
                s_rendererAPI->DisableDepth();
            }

        private:
            static Scope<RendererAPI> s_rendererAPI;
    };
}
