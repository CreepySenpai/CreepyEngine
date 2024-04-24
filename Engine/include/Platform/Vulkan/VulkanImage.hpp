#pragma once

#include <vulkan/vulkan.hpp>

namespace Creepy{

    class VulkanContext;

    struct VulkanImage {
        
        vk::Image ImageHandle;
        vk::ImageView ImageView;
        vk::DeviceMemory ImageMemory;
        uint32_t Width;
        uint32_t Height;

        static void CreateImage(VulkanContext* context, vk::ImageType imageType, uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags memoryFlags, bool isCreateView, vk::ImageAspectFlags aspect, VulkanImage &image) noexcept;
        
        static void CreateImageView(VulkanContext* context, vk::Format format, VulkanImage& image, vk::ImageAspectFlags aspect) noexcept;

        static void DestroyImage(VulkanContext* context, VulkanImage& image) noexcept;
    };

}