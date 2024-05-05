#pragma once

#include <span>
#include <vulkan/vulkan.hpp>

namespace Creepy {

    struct VulkanPipelineSpec;

    class VulkanPipeline
    {
        public:
            VulkanPipeline(const VulkanPipelineSpec& pipelineSpec) noexcept;

            void Destroy(const vk::Device logicalDev) noexcept;

            void Bind(const vk::CommandBuffer commandBuffer, vk::PipelineBindPoint bindingPoint) noexcept;

        private:
            vk::PipelineViewportStateCreateInfo createViewportState(const vk::Viewport& viewport, const vk::Rect2D& scissor) noexcept;

            vk::PipelineRasterizationStateCreateInfo createRasterizationState(bool isWireFrame) noexcept;

            vk::PipelineMultisampleStateCreateInfo createMultiSamplesState() noexcept;

            vk::PipelineDepthStencilStateCreateInfo createDepthStencilState() noexcept;

            vk::PipelineColorBlendStateCreateInfo createColorBlendState() noexcept;

            vk::PipelineDynamicStateCreateInfo createDynamicState() noexcept;

            vk::PipelineVertexInputStateCreateInfo createVertexInputState(std::span<const vk::VertexInputAttributeDescription> attributes) noexcept;

            vk::PipelineInputAssemblyStateCreateInfo createInputAssemblyState() noexcept;
        
        private:
            vk::Pipeline m_handle{nullptr};
            vk::PipelineLayout m_layout{nullptr};
    };
}