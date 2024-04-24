#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

namespace Creepy{

    struct VulkanSwapChainSupportInfo{
        vk::SurfaceCapabilitiesKHR Capabilities;
        std::vector<vk::SurfaceFormatKHR> Formats;
        std::vector<vk::PresentModeKHR> PresentModes;
    };

    struct VulkanDevice {
        vk::PhysicalDevice PhysicalDevice{nullptr};
        vk::Device LogicalDevice{nullptr};
        VulkanSwapChainSupportInfo SwapChainSupport{};
        int GraphicsFamilyIndex{};
        int PresentFamilyIndex{};
        int ComputeFamilyIndex{};
        int TransferFamilyIndex{};
        vk::PhysicalDeviceProperties PhysicalDeviceProperties;
        vk::PhysicalDeviceFeatures PhysicalDeviceFeatures;
        vk::PhysicalDeviceMemoryProperties PhysicalDeviceMemory;
        vk::Queue GraphicsQueue{};
        vk::Queue PresentQueue{};
        vk::Queue TransferQueue{};
        vk::Format DepthFormat{};
    };

}