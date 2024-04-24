#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>
#include <Platform/Vulkan/VulkanImage.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>

namespace Creepy{

    void VulkanImage::CreateImage(VulkanContext* context, vk::ImageType imageType, uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags memoryFlags, bool isCreateView, vk::ImageAspectFlags aspect, VulkanImage &image) noexcept {
        image.Width = width;
        image.Height = height;

        vk::ImageCreateInfo imageInfo{};
        imageInfo.flags = vk::ImageCreateFlags{};
        imageInfo.imageType = imageType;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 4;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;
        imageInfo.usage = usage;
        imageInfo.samples = vk::SampleCountFlagBits::e1;
        imageInfo.sharingMode = vk::SharingMode::eExclusive;

        VULKAN_CHECK_ERROR(image.ImageHandle = context->Devices.LogicalDevice.createImage(imageInfo));
        
        auto&& memoryRequire = context->Devices.LogicalDevice.getImageMemoryRequirements(image.ImageHandle);

        
        int&& memoryIndex = context->FindMemoryIndex(memoryRequire.memoryTypeBits, memoryFlags);
        vk::MemoryAllocateInfo allocInfo{};
        allocInfo.allocationSize = memoryRequire.size;
        allocInfo.memoryTypeIndex = memoryIndex;

        
        VULKAN_CHECK_ERROR(image.ImageMemory = context->Devices.LogicalDevice.allocateMemory(allocInfo));

        // TODO: config offset
        
        VULKAN_CHECK_ERROR(context->Devices.LogicalDevice.bindImageMemory(image.ImageHandle, image.ImageMemory, 0));

        if(isCreateView){
            image.ImageView = nullptr;
            VulkanImage::CreateImageView(context, format, image, aspect);
        }
    }
        
    void VulkanImage::CreateImageView(VulkanContext* context, vk::Format format, VulkanImage& image, vk::ImageAspectFlags aspect) noexcept {

        vk::ImageViewCreateInfo imageViewInfo{};
        imageViewInfo.flags = vk::ImageViewCreateFlags{};
        imageViewInfo.image = image.ImageHandle;
        imageViewInfo.viewType = vk::ImageViewType::e2D;
        imageViewInfo.format = format;
        imageViewInfo.subresourceRange.aspectMask = aspect;

        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.levelCount = 1;
        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.layerCount = 1;

        VULKAN_CHECK_ERROR(image.ImageView = context->Devices.LogicalDevice.createImageView(imageViewInfo));

    }

    void VulkanImage::DestroyImage(VulkanContext* context, VulkanImage& image) noexcept {
        context->Devices.LogicalDevice.destroyImageView(image.ImageView);
        image.ImageView = nullptr;
        context->Devices.LogicalDevice.freeMemory(image.ImageMemory);
        image.ImageMemory = nullptr;
        context->Devices.LogicalDevice.destroyImage(image.ImageHandle);
        image.ImageHandle = nullptr;
    }

}