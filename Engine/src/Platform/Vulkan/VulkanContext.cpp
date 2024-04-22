#include <vector>
#include <GLFW/glfw3.h>
#include <CreepyEngine/Core/Application.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>

namespace Creepy {

    // TODO: Move to utils
    static VkBool32 VKAPI_PTR debugUtilsCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* data, void* userData) {

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

    VulkanContext::VulkanContext(GLFWwindow* windowHandle) noexcept : m_windowHandle{windowHandle} {}

    void VulkanContext::Init() noexcept
    {
        
        // TODO: Custom ALloc
        m_allocator = nullptr;

        // Create Instance
        initInstance();

        // Create Debugger
        initDebugMessage();

        // Create Surface
        initSurface();

        // Create Device
        initDevice();
    }

    void VulkanContext::initInstance() noexcept {
        uint32_t version{0};
        auto _ = vk::enumerateInstanceVersion(&version);

        vk::ApplicationInfo appInfo("Test", version, "Creepy Engine Test", version, version);

        // Get Require Extension
        uint32_t extensionGLFWRequire{0};
        auto glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionGLFWRequire);
        std::vector<const char*> extensions{glfwExtensions, glfwExtensions + extensionGLFWRequire};
        extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        for(auto& ex : extensions){
            ENGINE_LOG_WARNING("Require Ex: {}", ex);
        }

        // Validation Layer
        std::vector<const char*> layers{
            "VK_LAYER_KHRONOS_validation"
        };

        auto supportLayers = vk::enumerateInstanceLayerProperties(m_dynamicLoader);

        for(auto& layer : supportLayers){
            ENGINE_LOG_WARNING("Support Layer: {}", layer.layerName.data());
        }

        vk::InstanceCreateInfo instanceInfo{};
        instanceInfo.flags = vk::InstanceCreateFlags{};
        instanceInfo.pApplicationInfo = &appInfo;
        instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        instanceInfo.ppEnabledExtensionNames = extensions.data();
        instanceInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
        instanceInfo.ppEnabledLayerNames = layers.data();

        VULKAN_CHECK_ERROR(m_instance = vk::createInstance(instanceInfo, m_allocator, m_dynamicLoader));

        if(m_instance){
            ENGINE_LOG_INFO("Create Vulkan Instance Success");
        }
        else {
            ENGINE_LOG_WARNING("Create Vulkan Instance Fail");
        }
    }

    void VulkanContext::initDebugMessage() noexcept {
        vk::DebugUtilsMessengerCreateInfoEXT debugInfo{};
        debugInfo.flags = vk::DebugUtilsMessengerCreateFlagsEXT{};
        debugInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
        debugInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
        debugInfo.pUserData = nullptr;
        debugInfo.pfnUserCallback = debugUtilsCallback;

        m_debugUtils = m_instance.createDebugUtilsMessengerEXT(debugInfo, m_allocator, m_dynamicLoader);
    }

    void VulkanContext::initSurface() noexcept {
        VkSurfaceKHR surfaceHandle;
        
        if(glfwCreateWindowSurface(m_instance, m_windowHandle, nullptr, &surfaceHandle) != VK_SUCCESS) {
            ENGINE_LOG_ERROR("Cannot Create Surface");
        }

        m_surface = vk::SurfaceKHR{surfaceHandle};
    }

    void VulkanContext::initDevice() noexcept {
        Device::CreateVulkanDevice(this);
    }

    void VulkanContext::SwapBuffers() noexcept
    {

    }

    void VulkanContext::ShutDown() noexcept {
        
        Device::DestroyDevice(this);

        m_dynamicLoader.vkDestroyDebugUtilsMessengerEXT(m_instance, static_cast<VkDebugUtilsMessengerEXT>(m_debugUtils), nullptr);

        m_instance.destroySurfaceKHR(m_surface);
        m_instance.destroy();
    }

}