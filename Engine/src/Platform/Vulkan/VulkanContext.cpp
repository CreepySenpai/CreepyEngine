// #include <CreepyEngine/Core/Core.hpp>
#include <GLFW/glfw3.h>
#include <Platform/Vulkan/VulkanContext.hpp>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>

namespace Creepy {

    // TODO: Move to utils
    static VkBool32 VKAPI_PTR debugUtilsCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* data, void* userData) {

        switch(severity){
            
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:{
                std::clog << "Vulkan Debug Info: " << data->pMessage << '\n';
                break;
            }

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:{
                std::clog << "Vulkan Debug Trace: " << data->pMessage << '\n';
                break;
            }

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:{
                std::clog << "Vulkan Debug Warning: " << data->pMessage << '\n';
                break;
            }

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:{
                std::clog << "Vulkan Debug Error: " << data->pMessage << '\n';
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
        std::clog << "Create Instance\n";

        // Create Instance
        initInstance();
        std::clog << "Create Debug\n";

        // Create Debugger
        initDebugMessage();
        std::clog << "Create Surface\n";

        // Create Surface
        initSurface();

        std::clog << "Create Device\n";

        // Create Device
        initDevice();

        std::clog << "Create SwapChain\n";

        // Create SwapChain
        initSwapChain();
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
            std::clog << "Require Ex: " << ex << '\n';
        }

        // Validation Layer
        std::vector<const char*> layers{
            "VK_LAYER_KHRONOS_validation"
        };

        auto supportLayers = vk::enumerateInstanceLayerProperties();

        for(auto& layer : supportLayers){
            std::clog << "Support Layer: " << layer.layerName.data() << '\n';
        }

        vk::InstanceCreateInfo instanceInfo{};
        instanceInfo.flags = vk::InstanceCreateFlags{};
        instanceInfo.pApplicationInfo = &appInfo;
        instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        instanceInfo.ppEnabledExtensionNames = extensions.data();
        instanceInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
        instanceInfo.ppEnabledLayerNames = layers.data();
        
        std::clog << "Gonna Create Instance\n";

        VULKAN_CHECK_ERROR(Instance = vk::createInstance(instanceInfo, nullptr));

        DynamicLoader = vk::DispatchLoaderDynamic(Instance, vkGetInstanceProcAddr);

    }

    void VulkanContext::initDebugMessage() noexcept {
        vk::DebugUtilsMessengerCreateInfoEXT debugInfo{
            vk::DebugUtilsMessengerCreateFlagsEXT{},
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
            debugUtilsCallback,
            nullptr
        };
        
        std::clog << "Gonna Create Debug\n";
        
        m_debugUtils = Instance.createDebugUtilsMessengerEXT(debugInfo, nullptr, DynamicLoader);

        std::clog << "Create Vulkan Debug\n";
    }

    void VulkanContext::initSurface() noexcept {
        VkSurfaceKHR surfaceHandle;
        
        if(glfwCreateWindowSurface(Instance, m_windowHandle, nullptr, &surfaceHandle) != VK_SUCCESS) {
            std::clog << "Cannot Create Surface\n";
        }

        Surface = vk::SurfaceKHR{surfaceHandle};

        std::clog << "Created Vulkan Surface\n";
    }

    void VulkanContext::initDevice() noexcept {
        Device::CreateVulkanDevice(this);
        std::clog << "Created Vulkan Device\n";
    }

    void VulkanContext::initSwapChain() noexcept {
        VulkanSwapchain::CreateSwapChain(this, 600, 600, Swapchain);
    }

    void VulkanContext::SwapBuffers() noexcept
    {

    }

    void VulkanContext::ShutDown() noexcept {
        std::clog << "Call Destroy Vulkan\n";

        std::clog << "Call Destroy SwapChain\n";
        VulkanSwapchain::DestroySwapChain(this, Swapchain);
        
        Device::DestroyDevice(this);

        std::clog << "Call Destroy Device\n";

        DynamicLoader.vkDestroyDebugUtilsMessengerEXT(Instance, static_cast<VkDebugUtilsMessengerEXT>(m_debugUtils), nullptr);

        std::clog << "Call Destroy Debug\n";

        Instance.destroySurfaceKHR(Surface);

        std::clog << "Call Destroy Surface\n";
        Instance.destroy();
        std::clog << "Destroy Vulkan\n";
    }

    int VulkanContext::FindMemoryIndex(uint32_t filterType, vk::MemoryPropertyFlags memoryFlags) noexcept {

        auto&& property = Devices.PhysicalDevice.getMemoryProperties();
        
        for(uint32_t i{}; i < property.memoryTypeCount; ++i){
            if(filterType & (1 << i) && (property.memoryTypes[i].propertyFlags & memoryFlags) == memoryFlags){
                return i;
            }
        }

        return -1;
    }

}