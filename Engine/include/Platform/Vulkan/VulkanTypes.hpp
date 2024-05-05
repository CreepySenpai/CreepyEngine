#pragma once

#include <vector>
#include <span>
#include <memory>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

namespace Creepy {

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T>
    using Scope = std::unique_ptr<T>;

    struct VulkanSwapChainSupportInfo{
        vk::SurfaceCapabilitiesKHR Capabilities;
        std::vector<vk::SurfaceFormatKHR> Formats;
        std::vector<vk::PresentModeKHR> PresentModes;
    };
    
    struct VulkanImageSpec{
        vk::ImageType ImageType;
        uint32_t Width{};
        uint32_t Height{};
        vk::Format Format;
        vk::ImageTiling Tiling;
        vk::ImageUsageFlags Usage;
        vk::MemoryPropertyFlags MemoryFlags;
        vk::ImageAspectFlags Aspect;
        vk::Device LogicalDev;
        bool IsCreateView;
    };

    struct VulkanRenderPassSpec{
        glm::vec4 RenderArea;
        glm::vec4 ClearColor;
        float Depth{};
        uint32_t Stencil{};
        vk::Device LogicalDev;
    };

    struct VulkanFrameBufferSpec{
        uint32_t Width{};
        uint32_t Height{};
        vk::RenderPass RenderPassHandle;
        std::vector<vk::ImageView> Attachments;
        vk::Device LogicalDev;
    };

    struct VulkanSwapChainSpec{
        uint32_t Width{};
        uint32_t Height{};
        uint32_t GraphicFamilyIndex{};
        uint32_t PresentFamilyIndex{};
        vk::SurfaceKHR Surface{};
        vk::Device LogicalDev{};
    };

    struct VulkanPipelineSpec{
        vk::VertexInputBindingDescription Description;
        vk::RenderPass RenderPass;
        std::span<const vk::VertexInputAttributeDescription> Attributes;
        std::span<const vk::DescriptorSetLayout> DescriptorSetLayouts;
        std::span<const vk::PipelineShaderStageCreateInfo> ShaderStages;
        vk::Viewport Viewport;
        vk::Rect2D Scissor;
        bool IsWireFrame;
    };

    struct VulkanBufferSpec{
        uint32_t Size;
        vk::BufferUsageFlags BufferUsage;
        vk::SharingMode SharingMode;
        vk::MemoryPropertyFlags MemoryProperty;
    };

    struct VulkanDescriptorSetLayoutSpec{
        uint32_t Binding{};
        uint32_t DescriptorCount{};
        vk::DescriptorType DescriptorType;
        vk::ShaderStageFlagBits StageFlags;
    };

    struct VulkanWriteDescriptorSetSpec{
        uint32_t DescriptorSetIndex;
        vk::DeviceSize Offset;
        vk::DeviceSize Range;
        vk::DescriptorType DescriptorType;
        uint32_t DescriptorCount;
    };
}