#include <CreepyEngine/Utils/VulkanUtils.hpp>
#include <CreepyEngine/Core/Core.hpp>

namespace Creepy{

    vk::Bool32 VulkanUtils::VulkanDebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* data, void* userData) {
        switch(severity){
            
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:{
                ENGINE_LOG_INFO("Vulkan Debug Info: {}", data->pMessage);
                break;
            }

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:{
                ENGINE_LOG_TRACE("Vulkan Debug Trace: {}", data->pMessage);
                break;
            }

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:{
                ENGINE_LOG_WARNING("Vulkan Debug Warning: {}", data->pMessage);
                break;
            }

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:{
                ENGINE_LOG_ERROR("Vulkan Debug Error: {}", data->pMessage);
                break;
            }
        }

        return VK_FALSE;
    }

    void VulkanUtils::LogInfo(const std::string& message) noexcept {
        ENGINE_LOG_INFO(message);
    }
    void VulkanUtils::LogTrace(const std::string& message) noexcept {
        ENGINE_LOG_TRACE(message);
    }

    void VulkanUtils::LogWarning(const std::string& message) noexcept {
        ENGINE_LOG_WARNING(message);
    }

    void VulkanUtils::LogError(const std::string& message) noexcept {
        ENGINE_LOG_ERROR(message);
    }
    
}