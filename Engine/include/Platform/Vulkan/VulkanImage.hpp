#pragma once

#include <vulkan/vulkan.hpp>

namespace Creepy{

    struct VulkanImageSpec;

    class VulkanImage
    {
        public:
            VulkanImage(const VulkanImageSpec& spec) noexcept;

            constexpr vk::ImageView GetImageView() const noexcept {
                return m_imageView;
            }

            void Destroy(vk::Device logicalDev) noexcept;

        private:
            void createImageView(vk::Device logicalDev, vk::Format format, vk::ImageAspectFlags aspect) noexcept;

        private:
            vk::Image m_handle{nullptr};
            vk::ImageView m_imageView{nullptr};
            vk::DeviceMemory m_imageMemory{nullptr};
            uint32_t m_width{};
            uint32_t m_height{};
    };
}