#include <Platform/Vulkan/VulkanBuffer.hpp>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <Platform/Vulkan/VulkanTypes.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>

namespace Creepy {

    VulkanBuffer::VulkanBuffer(const VulkanBufferSpec& bufferSpec) noexcept : m_bufferSize{bufferSpec.Size} {
        vk::BufferCreateInfo bufferInfo{};
        bufferInfo.flags = vk::BufferCreateFlags{};
        bufferInfo.size = bufferSpec.Size;
        bufferInfo.usage = bufferSpec.BufferUsage;
        bufferInfo.sharingMode = bufferSpec.SharingMode;

        VULKAN_CHECK_ERROR(m_handle = VulkanDevice::GetLogicalDevice().createBuffer(bufferInfo));

        const auto&& memoryRequire = VulkanDevice::GetLogicalDevice().getBufferMemoryRequirements(m_handle);
        int memoryIndex = VulkanDevice::FindMemoryIndex(memoryRequire.memoryTypeBits, bufferSpec.MemoryProperty);

        if(memoryIndex == -1){
            VulkanUtils::LogError("False to find memory index");
        }

        vk::MemoryAllocateInfo allocInfo{};
        allocInfo.allocationSize = memoryRequire.size;
        allocInfo.memoryTypeIndex = static_cast<uint32_t>(memoryIndex);

        VULKAN_CHECK_ERROR(m_memory = VulkanDevice::GetLogicalDevice().allocateMemory(allocInfo));

        this->Bind();
    }

    void VulkanBuffer::Bind() noexcept
    {
        VULKAN_CHECK_ERROR(VulkanDevice::GetLogicalDevice().bindBufferMemory(m_handle, m_memory, 0));
    }

    void VulkanBuffer::Destroy() noexcept
    {
        VulkanDevice::GetLogicalDevice().freeMemory(m_memory);
        m_memory = nullptr;
        VulkanDevice::GetLogicalDevice().destroyBuffer(m_handle);
        m_handle = nullptr;
    }

    void VulkanBuffer::lockMemory(uint32_t offset, uint32_t size) noexcept
    {
        VULKAN_CHECK_ERROR(m_bufferData = VulkanDevice::GetLogicalDevice().mapMemory(m_memory, offset, size == 0 ? m_bufferSize : size));
    }

    void VulkanBuffer::unlockMemory() noexcept
    {
        VulkanDevice::GetLogicalDevice().unmapMemory(m_memory);
        m_bufferData = nullptr;
    }

    void VulkanBuffer::SetData(const void* data, uint32_t size) noexcept {
        lockMemory();

        std::memcpy(m_bufferData, data, static_cast<size_t>(size));

        unlockMemory();
    }

    void VulkanBuffer::SetData(const void* data, uint32_t size, uint32_t offset) noexcept {
        lockMemory(offset, size);

        std::memcpy(m_bufferData, data, static_cast<size_t>(size));

        unlockMemory();
    }

    void VulkanBuffer::CopyBuffer(const VulkanBuffer& src, const VulkanBuffer& dest, vk::DeviceSize size, const vk::CommandBuffer commandBuffer) noexcept {
        vk::BufferCopy bufferCopy{};
        bufferCopy.size = size;
        bufferCopy.srcOffset = 0;
        bufferCopy.dstOffset = 0;

        commandBuffer.copyBuffer(src.GetHandle(), dest.GetHandle(), bufferCopy);
    }


    VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size) noexcept
       : m_buffer{{size, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive, vk::MemoryPropertyFlagBits::eDeviceLocal}}
    {
        
    }

    VulkanVertexBuffer::VulkanVertexBuffer(const void* data, uint32_t size) noexcept : VulkanVertexBuffer{size} {
        this->SetData(data, size);
    }

    void VulkanVertexBuffer::Bind() noexcept {
        m_buffer.Bind();
    }

    void VulkanVertexBuffer::Destroy() noexcept {
        m_buffer.Destroy();
    }

    void VulkanVertexBuffer::SetData(const void* data, uint32_t size) noexcept {
        m_buffer.SetData(data, size);
    }

    // Index Buffer

    VulkanIndexBuffer::VulkanIndexBuffer(uint32_t count) noexcept
        : m_buffer{{count * static_cast<uint32_t>(sizeof(uint32_t)), vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive, vk::MemoryPropertyFlagBits::eDeviceLocal}}
    {
        
    }

    VulkanIndexBuffer::VulkanIndexBuffer(const void* data, uint32_t count) noexcept : VulkanIndexBuffer{count} {
        this->SetData(data, count);
    }

    void VulkanIndexBuffer::Bind() noexcept {
        m_buffer.Bind();
    }

    void VulkanIndexBuffer::Destroy() noexcept {
        m_buffer.Destroy();
    }

    void VulkanIndexBuffer::SetData(const void* data, uint32_t count) noexcept {
        m_buffer.SetData(data, count * sizeof(uint32_t));
    }


    // Uniform Buffer

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

    void VulkanUniformBuffer::SetData(const void* data, uint32_t size) noexcept {
        m_buffer.SetData(data, size);
    }

}