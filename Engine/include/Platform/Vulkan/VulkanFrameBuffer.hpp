#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>


namespace Creepy {

    struct VulkanFrameBufferSpec;

    class VulkanFrameBuffer
    {
        public:
            VulkanFrameBuffer(const VulkanFrameBufferSpec& frameBufferSpec) noexcept;

            void Destroy(const vk::Device logicalDev) noexcept;

            constexpr vk::Framebuffer GetHandle() const noexcept {
                return m_handle;
            }

            void BeginFrame() noexcept;

            void EndFrame() noexcept;
        private:
            vk::Framebuffer m_handle;
            // std::vector<vk::ImageView> m_imageViews;
    };

    struct VulkanFrame{
        vk::Framebuffer FrameHandle;
    };

    class VulkanFrameBufferT{
        public:
            VulkanFrameBufferT(const VulkanFrameBufferSpec& frameBufferSpec) noexcept;

            void Destroy() noexcept;

            void BeginFrame() noexcept;

            void EndFrame() noexcept;
        private:
            std::vector<VulkanFrame> m_frames;
            
    };
   
}