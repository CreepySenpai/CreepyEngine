#pragma once

#include <string>
#include <vulkan/vulkan.hpp>

namespace Creepy {

    struct VulkanUtils{
        static VkBool32 VKAPI_PTR VulkanDebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* data, void* userData);

        static void Log(std::string message) noexcept;
    };
}