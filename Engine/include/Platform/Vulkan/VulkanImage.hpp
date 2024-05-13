#pragma once

#include <vulkan/vulkan.hpp>

namespace Creepy{

    struct VulkanImageSpec;

    class VulkanImage
    {
        public:
            VulkanImage() noexcept = default;
            VulkanImage(const VulkanImageSpec& spec) noexcept;

            constexpr vk::Image GetImage() const noexcept {
                return m_handle;
            }

            constexpr vk::ImageView GetImageView() const noexcept {
                return m_imageView;
            }

            void Destroy(const vk::Device logicalDev) noexcept;

        private:
            void createImageView(const vk::Device logicalDev, vk::Format format, vk::ImageAspectFlags aspect) noexcept;

        private:
            vk::Image m_handle{nullptr};
            vk::ImageView m_imageView{nullptr};
            vk::DeviceMemory m_imageMemory{nullptr};
            uint32_t m_width{};
            uint32_t m_height{};
    };
}