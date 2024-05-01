#pragma once

#include <string>
#include <vulkan/vulkan.hpp>

namespace Creepy {

    struct VulkanUtils{
        static vk::Bool32 VKAPI_PTR VulkanDebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* data, void* userData);

        static void LogInfo(const std::string& message) noexcept;
        static void LogTrace(const std::string& message) noexcept;
        static void LogWarning(const std::string& message) noexcept;
        static void LogError(const std::string& message) noexcept;
    };
}