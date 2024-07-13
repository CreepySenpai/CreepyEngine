#include <Platform2/Vulkan/VulkanBuffer.hpp>


namespace Creepy {
    VulkanVertexBuffer2::VulkanVertexBuffer2(uint32_t size) noexcept
    // : m_buffer{{size, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive, vk::MemoryPropertyFlagBits::eDeviceLocal}}
    {

    }

    VulkanVertexBuffer2::VulkanVertexBuffer2(const void* data, uint32_t size) noexcept {

    }

    void VulkanVertexBuffer2::BindImpl() const noexcept {

    }

    void VulkanVertexBuffer2::SetDataImpl(const void* data, uint32_t size) noexcept {

    }

    /////////////////////////////////////////////////////

    VulkanIndexBuffer2::VulkanIndexBuffer2(const void* data, uint32_t count) noexcept {

    }

    void VulkanIndexBuffer2::BindImpl() const noexcept {

    }
    
    void VulkanIndexBuffer2::SetDataImpl(const void* data, uint32_t count) noexcept {

    }
}