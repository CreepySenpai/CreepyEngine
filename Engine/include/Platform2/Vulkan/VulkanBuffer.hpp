#pragma once

#include <CreepyEngine/Renderer/Buffer.hpp>
#include <vulkan/vulkan.hpp>

namespace Creepy {

    struct VulkanBufferSpec;

    class VulkanBuffer
    {
        public:

            VulkanBuffer() noexcept = default;

            VulkanBuffer(const VulkanBufferSpec& bufferSpec) noexcept;

            void Bind() const noexcept;

            void Destroy() noexcept;

            void SetData(const void* data, uint32_t size) noexcept;

            void SetData(const void* data, uint32_t size, uint32_t offset) noexcept;

            constexpr vk::Buffer GetHandle() const noexcept {
                return m_handle;
            }

            vk::DeviceAddress GetBufferDeviceAddress() noexcept;

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

    class VulkanVertexBuffer2 : public VertexBuffer2 {
        friend class VertexBuffer2;

        public:
            VulkanVertexBuffer2(uint32_t size) noexcept;
            VulkanVertexBuffer2(const void* data, uint32_t size) noexcept;

            constexpr explicit operator VulkanBuffer() noexcept {
                return m_buffer;
            }
        private:
            void BindImpl() const noexcept;

            void UnBindImpl() const noexcept {}
            
            const BufferLayout& GetLayoutImpl() const noexcept {
                // TODO: Fix
                BufferLayout layout{};
                return layout;
            }

            void SetLayoutImpl(const BufferLayout& layout) noexcept {}

            void SetDataImpl(const void* data, uint32_t size) noexcept;
        private:
            VulkanBuffer m_buffer;
    };


    class VulkanIndexBuffer2 : public IndexBuffer2{
        friend class IndexBuffer2;
        
        public:
            VulkanIndexBuffer2(const void* data, uint32_t count) noexcept;

            constexpr explicit operator VulkanBuffer() noexcept {
                return m_buffer;
            }

        private:
            void BindImpl() const noexcept;

            void UnBindImpl() const noexcept {}

            uint32_t GetCountImpl() const noexcept {
                return m_count;
            }
        
            void SetDataImpl(const void* data, uint32_t count) noexcept;
        private:
            VulkanBuffer m_buffer;
            uint32_t m_count{};
    };
}