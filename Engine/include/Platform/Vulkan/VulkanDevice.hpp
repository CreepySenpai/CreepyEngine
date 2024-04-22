#pragma once

#include "VulkanTypes.hpp"

namespace Creepy {

    class VulkanContext;

    struct Device {
        
        static bool CreateVulkanDevice(VulkanContext* context) noexcept;

        static void DestroyDevice(VulkanContext* context) noexcept;

    };

}