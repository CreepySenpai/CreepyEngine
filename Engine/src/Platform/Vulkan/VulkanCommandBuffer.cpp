#include <Platform/Vulkan/VulkanCommandBuffer.hpp>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>

namespace Creepy {

    VulkanCommandBuffer::VulkanCommandBuffer(vk::Device logicalDev, vk::CommandPool commandPool, vk::CommandBufferLevel level) noexcept {

        vk::CommandBufferAllocateInfo allocInfo{};
        allocInfo.commandPool = commandPool;
        allocInfo.level = level;
        allocInfo.commandBufferCount = 1;
        
        m_state = CommandBufferState::NOT_ALLOCATED;

        // Only Alloc 1 Command Buffer
        VULKAN_CHECK_ERROR(m_handle = logicalDev.allocateCommandBuffers(allocInfo).at(0));
        
        m_state = CommandBufferState::READY;
    }
    
    void VulkanCommandBuffer::Free(vk::Device logicalDev, vk::CommandPool commandPool) noexcept {
        logicalDev.freeCommandBuffers(commandPool, m_handle);
        m_state = CommandBufferState::NOT_ALLOCATED;
    }

    void VulkanCommandBuffer::Begin(bool isSingleUse, bool isRenderPassContinue, bool isSimultaneousUse) noexcept {
        vk::CommandBufferBeginInfo beginInfo{};
        beginInfo.flags = vk::CommandBufferUsageFlags{};
        if(isSingleUse){
            beginInfo.flags != vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        }
        if(isRenderPassContinue){
            beginInfo.flags != vk::CommandBufferUsageFlagBits::eRenderPassContinue;
        }
        if(isSimultaneousUse){
            beginInfo.flags != vk::CommandBufferUsageFlagBits::eSimultaneousUse;
        }
        
        VULKAN_CHECK_ERROR(m_handle.begin(beginInfo));
        
        m_state = CommandBufferState::RECORDING;
    }

    void VulkanCommandBuffer::UpdateSubmitted() noexcept {
        m_state = CommandBufferState::SUBMITTED;
    }

    void VulkanCommandBuffer::End() noexcept {
        VULKAN_CHECK_ERROR(m_handle.end());
        m_state = CommandBufferState::RECORDING_ENDED;
    }

    void VulkanCommandBuffer::Reset() noexcept {
        m_handle.reset();
        m_state = CommandBufferState::READY;
    }

}