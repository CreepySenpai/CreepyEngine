#include <CreepyEngine/Core/Core.hpp>
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
                ENGINE_LOG_ERROR("Vulkan Debug Info: {}", data->pMessage);
                break;
            }

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:{
                ENGINE_LOG_ERROR("Vulkan Debug Trace: {}", data->pMessage);
                break;
            }

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:{
                ENGINE_LOG_ERROR("Vulkan Debug Warning: {}", data->pMessage);
                break;
            }

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:{
                ENGINE_LOG_ERROR("Vulkan Debug Error: {}", data->pMessage);
                break;
            }
        }

        return VK_FALSE;
    }

    VulkanContext::VulkanContext(GLFWwindow* windowHandle) noexcept : m_windowHandle{windowHandle} {
        
    }

    void VulkanContext::Init() noexcept
    {
        
        // TODO: Custom ALloc
        Allocator = nullptr;
        ENGINE_LOG_WARNING("Create Instance");

        // Create Instance
        initInstance();
        ENGINE_LOG_WARNING("Create Debug");

        // Create Debugger
        initDebugMessage();

        ENGINE_LOG_WARNING("Create Surface");

        // Create Surface
        initSurface();

        ENGINE_LOG_WARNING("Create Device");

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

        auto supportLayers = vk::enumerateInstanceLayerProperties();

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
        
        ENGINE_LOG_WARNING("Gonna Create Instance");

        try{
            Instance = vk::createInstance(instanceInfo, nullptr);
        }
        catch(const vk::SystemError& e){
            ENGINE_LOG_ERROR("Vulkan Error: {}", e.what());
        }

        DynamicLoader = vk::DispatchLoaderDynamic(Instance, vkGetInstanceProcAddr);

    }

    void VulkanContext::initDebugMessage() noexcept {
        vk::DebugUtilsMessengerCreateInfoEXT debugInfo{
            vk::DebugUtilsMessengerCreateFlagsEXT{},
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
            debugUtilsCallback,
            nullptr
        };
        
        ENGINE_LOG_WARNING("Gonna Create Debug");
        
        m_debugUtils = Instance.createDebugUtilsMessengerEXT(debugInfo, nullptr, DynamicLoader);

        ENGINE_LOG_WARNING("Create Vulkan Debug");
    }

    void VulkanContext::initSurface() noexcept {
        VkSurfaceKHR surfaceHandle;
        
        if(glfwCreateWindowSurface(Instance, m_windowHandle, nullptr, &surfaceHandle) != VK_SUCCESS) {
            ENGINE_LOG_ERROR("Cannot Create Surface");
        }

        Surface = vk::SurfaceKHR{surfaceHandle};

        ENGINE_LOG_WARNING("Create Vulkan Surface");
    }

    void VulkanContext::initDevice() noexcept {
        Device::CreateVulkanDevice(this);
        ENGINE_LOG_WARNING("Create Vulkan Device");
    }

    void VulkanContext::SwapBuffers() noexcept
    {

    }

    void VulkanContext::ShutDown() noexcept {
        
        Device::DestroyDevice(this);

        DynamicLoader.vkDestroyDebugUtilsMessengerEXT(Instance, static_cast<VkDebugUtilsMessengerEXT>(m_debugUtils), nullptr);

        Instance.destroySurfaceKHR(Surface);
        Instance.destroy();

        ENGINE_LOG_WARNING("Destroy Vulkan");
    }

}