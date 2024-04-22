#pragma once

#include <CreepyEngine/Renderer/RendererAPI.hpp>


namespace Creepy{

    class VulkanRendererAPI : public RendererAPI
    {
        public:

            void Init() noexcept override;

            void SetClearColor(const glm::vec4& color) noexcept override;

            void Clear() noexcept override;

            void EnableDepth() noexcept override;

            void DisableDepth() noexcept override;

            // void DrawIndex(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) noexcept override;

            // void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount = 0) noexcept override;

            // void DrawInstances(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0, uint32_t instanceCount = 0) noexcept override;

            // void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept override;

            // void SetLineThickness(float thickness) noexcept override;
    };
}