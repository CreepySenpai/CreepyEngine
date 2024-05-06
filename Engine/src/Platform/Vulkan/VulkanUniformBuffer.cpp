#include <Platform/Vulkan/VulkanUniformBuffer.hpp>
#include <Platform/Vulkan/VulkanTypes.hpp>

namespace Creepy {

    VulkanUniformBuffer::VulkanUniformBuffer(uint32_t size) noexcept
        : m_buffer{{size, vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::SharingMode::eExclusive, vk::MemoryPropertyFlagBits::eDeviceLocal | vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent}}
    {

    }

    void VulkanUniformBuffer::Bind() noexcept {
        m_buffer.Bind();
    }

    void VulkanUniformBuffer::Destroy() noexcept {
        m_buffer.Destroy();
    }

    void VulkanUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset) noexcept {
        m_buffer.SetData(data, size, offset);
    }

}