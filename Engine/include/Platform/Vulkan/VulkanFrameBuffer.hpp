#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>


namespace Creepy{

    struct VulkanFrameBufferSpec;

    class VulkanFrameBuffer
    {
        public:
            VulkanFrameBuffer(const VulkanFrameBufferSpec& frameBufferSpec) noexcept;

            void Destroy(vk::Device logicalDev) noexcept;

            constexpr vk::Framebuffer GetHandle() const noexcept {
                return m_handle;
            }
        private:
            vk::Framebuffer m_handle;
            // std::vector<vk::ImageView> m_imageViews;
    };
   
}