#pragma once

#include <vulkan/vulkan.hpp>

namespace Creepy{

    struct VulkanDevice{
        vk::PhysicalDevice PhysicalDevice{nullptr};
        vk::Device LogicalDevice{nullptr};
    };

}