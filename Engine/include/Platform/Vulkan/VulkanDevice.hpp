#pragma once

#include "VulkanTypes.hpp"

namespace Creepy {

    class VulkanContext;

    struct Device {
        
        static bool CreateVulkanDevice(VulkanContext* context) noexcept;

        static void DestroyDevice(VulkanContext* context) noexcept;

        static void VulkanDeviceQuerySwapChainSupport(vk::PhysicalDevice physicDev, vk::SurfaceKHR surface, VulkanSwapChainSupportInfo& info) noexcept;

        static bool VulkanDeviceDetectDepthFormat(VulkanDevice& devices) noexcept;
    };

}