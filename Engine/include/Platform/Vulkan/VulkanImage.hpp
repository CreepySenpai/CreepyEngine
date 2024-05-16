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

            constexpr vk::Extent2D GetImageExtent() const noexcept {
                return vk::Extent2D{m_width, m_height};
            }

            void Destroy(const vk::Device logicalDev) noexcept;


            static void CopyImage(const vk::CommandBuffer commandBuffer, const vk::Image srcImage, const vk::Image dstImage, vk::Extent2D srcSize, vk::Extent2D dstSize) noexcept;

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