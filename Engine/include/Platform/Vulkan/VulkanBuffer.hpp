#pragma once

#include <vulkan/vulkan.hpp>


namespace Creepy {

    struct VulkanBufferSpec;

    class VulkanBuffer
    {
        public:

            VulkanBuffer() noexcept = default;

            VulkanBuffer(const VulkanBufferSpec& bufferSpec) noexcept;

            void Bind() noexcept;

            void Destroy() noexcept;

            void SetData(const void* data, uint32_t size) noexcept;

            void SetData(const void* data, uint32_t size, uint32_t offset) noexcept;

            constexpr vk::Buffer GetHandle() const noexcept {
                return m_handle;
            }

        public:
            static void CopyBuffer(const VulkanBuffer& src, const VulkanBuffer& dest, vk::DeviceSize size, const vk::CommandBuffer commandBuffer) noexcept;
        
        private:
            void lockMemory(uint32_t offset = 0, uint32_t size = 0) noexcept;
            void unlockMemory() noexcept;

        private:
            vk::Buffer m_handle{nullptr};
            vk::DeviceMemory m_memory{nullptr};
            uint32_t m_bufferSize{};
            void* m_bufferData{nullptr};
    };
    

    class VulkanVertexBuffer {
        public:
            VulkanVertexBuffer(uint32_t size) noexcept;
            VulkanVertexBuffer(const void* data, uint32_t size) noexcept;

            void Bind() noexcept;

            void Destroy() noexcept;

            void SetData(const void* data, uint32_t size) noexcept;

            constexpr explicit operator VulkanBuffer() noexcept {
                return m_buffer;
            }

        private:
            VulkanBuffer m_buffer;
    };


    class VulkanIndexBuffer {

        public:
            VulkanIndexBuffer(uint32_t count) noexcept;
            VulkanIndexBuffer(const void* data, uint32_t count) noexcept;

            void Bind() noexcept;
            
            void Destroy() noexcept;
        
            void SetData(const void* data, uint32_t count) noexcept;

            constexpr explicit operator VulkanBuffer() noexcept {
                return m_buffer;
            }

        private:
            VulkanBuffer m_buffer;
    };

    class VulkanUniformBuffer{
        public:
            VulkanUniformBuffer(uint32_t size) noexcept;

            void Bind() noexcept;
            void Destroy() noexcept;

            void SetData(const void* data, uint32_t size) noexcept;
            
        private:
            VulkanBuffer m_buffer;
    };

}