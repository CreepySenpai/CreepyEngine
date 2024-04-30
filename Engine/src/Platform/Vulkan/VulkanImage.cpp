#include <Platform/Vulkan/VulkanImage.hpp>
#include <Platform/Vulkan/VulkanTypes.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>

namespace Creepy {

    VulkanImage::VulkanImage(const VulkanImageSpec &spec) noexcept : m_width{spec.Width}, m_height{spec.Height}
    {
        vk::ImageCreateInfo imageInfo{};
        imageInfo.flags = vk::ImageCreateFlags{};
        imageInfo.imageType = spec.ImageType;
        imageInfo.extent.width = m_width;
        imageInfo.extent.height = m_height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 4;
        imageInfo.arrayLayers = 1;
        imageInfo.format = spec.Format;
        imageInfo.tiling = spec.Tiling;
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;
        imageInfo.usage = spec.Usage;
        imageInfo.samples = vk::SampleCountFlagBits::e1;
        imageInfo.sharingMode = vk::SharingMode::eExclusive;

        VULKAN_CHECK_ERROR(m_handle = spec.LogicalDev.createImage(imageInfo));
        
        auto&& memoryRequire = spec.LogicalDev.getImageMemoryRequirements(m_handle);
        
        int&& memoryIndex = VulkanContext::GetInstance()->FindMemoryIndex(memoryRequire.memoryTypeBits, spec.MemoryFlags);

        vk::MemoryAllocateInfo allocInfo{};
        allocInfo.allocationSize = memoryRequire.size;
        allocInfo.memoryTypeIndex = memoryIndex;

        VULKAN_CHECK_ERROR(m_imageMemory = spec.LogicalDev.allocateMemory(allocInfo));

        // TODO: config offset
        spec.LogicalDev.bindImageMemory(m_handle, m_imageMemory, 0);

        if(spec.IsCreateView){
            createImageView(spec.LogicalDev, spec.Format, spec.Aspect);
        }

    }

    void VulkanImage::Destroy(vk::Device logicalDev) noexcept
    {
        logicalDev.destroyImageView(m_imageView);
        m_imageView = nullptr;
        logicalDev.freeMemory(m_imageMemory);
        m_imageMemory = nullptr;
        logicalDev.destroyImage(m_handle);
        m_handle = nullptr;
    }

    void VulkanImage::createImageView(vk::Device logicalDev, vk::Format format, vk::ImageAspectFlags aspect) noexcept {
        vk::ImageViewCreateInfo imageViewInfo{};
        imageViewInfo.flags = vk::ImageViewCreateFlags{};
        imageViewInfo.image = m_handle;
        imageViewInfo.viewType = vk::ImageViewType::e2D;
        imageViewInfo.format = format;
        imageViewInfo.subresourceRange.aspectMask = aspect;

        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.levelCount = 1;
        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.layerCount = 1;

        VULKAN_CHECK_ERROR(m_imageView = logicalDev.createImageView(imageViewInfo));
    }
}