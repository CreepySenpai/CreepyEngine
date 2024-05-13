#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <CreepyEngine/Renderer/FrameBuffer.hpp>
#include "VulkanImage.hpp"


namespace Creepy {

    class VulkanFrameBuffer
    {
        public:
            VulkanFrameBuffer(const FrameBufferSpecification& frameBufferSpec) noexcept;
            void Invalidate() noexcept;

            void Resize(uint32_t width, uint32_t height) noexcept;

            void Destroy(const vk::Device logicalDev) noexcept;

            void BeginFrame() noexcept;

            void EndFrame() noexcept;
        private:
            
            FrameBufferSpecification m_frameBufferSpecs;
            // vk::Format m_colorAttachmentFormat;
            // vk::Format m_depthTextureFormat;
            std::vector<FrameBufferTextureSpecification> m_colorTextureFormats;
            FrameBufferTextureSpecification m_depthTextureFormat;

            std::vector<VulkanImage> m_colorAttachments;
            VulkanImage m_depthAttachment;
    };
   
}