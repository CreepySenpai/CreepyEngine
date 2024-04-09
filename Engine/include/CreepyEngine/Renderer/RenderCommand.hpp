#pragma once
#include <memory>
#include <CreepyEngine/Core/Core.hpp>
#include "RendererAPI.hpp"

namespace Creepy
{
    class RenderCommand
    {
        public:

            constexpr static void Init() noexcept {
                s_rendererAPI->Init();
            }
            
            constexpr static void DrawIndex(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) noexcept {
                s_rendererAPI->DrawIndex(vertexArray, indexCount);
            }

            constexpr static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount = 0) noexcept {
                s_rendererAPI->DrawLines(vertexArray, vertexCount);
            }

            constexpr static void DrawInstances() noexcept {
                s_rendererAPI->DrawInstances();
            }

            constexpr static void SetClearColor(const glm::vec4& color) noexcept {
                s_rendererAPI->SetClearColor(color);
            }

            constexpr static void Clear() noexcept {
                s_rendererAPI->Clear();
            }

            constexpr static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept {
                s_rendererAPI->SetViewPort(x, y, width, height);
            }

            constexpr static void SetLineThickness(float thickness) noexcept {
                s_rendererAPI->SetLineThickness(thickness);
            }

            constexpr static void EnableDepth() noexcept {
                s_rendererAPI->EnableDepth();
            }

            constexpr static void DisableDepth() noexcept {
                s_rendererAPI->DisableDepth();
            }

        private:
            static Scope<RendererAPI> s_rendererAPI;
    };
}
