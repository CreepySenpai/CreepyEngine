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

    class VulkanVertexBuffer : public VertexBuffer {
        public:
            VulkanVertexBuffer(uint32_t size) noexcept;
            VulkanVertexBuffer(const void* data, uint32_t size) noexcept;

            void Bind() const noexcept override;

            void UnBind() const noexcept override {}
            
            const BufferLayout& GetLayout() const noexcept override {
                // TODO: Fix
                BufferLayout layout{};
                return layout;
            }

            void SetLayout(const BufferLayout& layout) noexcept {};

            void Destroy() noexcept;

            void SetData(const void* data, uint32_t size) noexcept;

            constexpr explicit operator VulkanBuffer() noexcept {
                return m_buffer;
            }

        private:
            VulkanBuffer m_buffer;
    };


    class VulkanIndexBuffer : public IndexBuffer {

        public:
            VulkanIndexBuffer(const void* data, uint32_t count) noexcept;

            void Bind() const noexcept override;
            
            void Destroy() noexcept;

            void UnBind() const noexcept override {}

            uint32_t GetCount() const noexcept override {
                return m_count;
            }
        
            void SetData(const void* data, uint32_t count) noexcept;

            constexpr explicit operator VulkanBuffer() noexcept {
                return m_buffer;
            }

        private:
            VulkanBuffer m_buffer;
            uint32_t m_count{};
    };
}