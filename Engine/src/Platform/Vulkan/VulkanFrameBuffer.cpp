#include <Platform/Vulkan/VulkanFrameBuffer.hpp>
#include <Platform/Vulkan/VulkanTypes.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>

namespace Creepy {

    VulkanFrameBuffer::VulkanFrameBuffer(const VulkanFrameBufferSpec& frameBufferSpec) noexcept {
        vk::FramebufferCreateInfo frameInfo{};
        frameInfo.flags = vk::FramebufferCreateFlags{};
        frameInfo.attachmentCount = static_cast<uint32_t>(frameBufferSpec.Attachments.size());
        frameInfo.pAttachments = frameBufferSpec.Attachments.data();
        frameInfo.width = frameBufferSpec.Width;
        frameInfo.height = frameBufferSpec.Height;
        frameInfo.layers = 1;
        frameInfo.renderPass = frameBufferSpec.RenderPassHandle;
        
        VULKAN_CHECK_ERROR(m_handle = frameBufferSpec.LogicalDev.createFramebuffer(frameInfo));
    }

    void VulkanFrameBuffer::Destroy(vk::Device logicalDev) noexcept {
        logicalDev.destroyFramebuffer(m_handle);
    }

}