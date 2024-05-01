#include <iostream>
#include <utility>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>
#include <CreepyEngine/Utils/VulkanUtils.hpp>

namespace Creepy{

    VulkanDevice::VulkanDevice() noexcept {
        this->initDevice();
    }

    void VulkanDevice::DestroyDevice() noexcept {
        m_logicalDevice.destroyCommandPool(GraphicCommandPool);
        
        m_logicalDevice.destroy();
    }


    VulkanSwapChainSupportInfo VulkanDevice::QuerySwapChainSupport(vk::SurfaceKHR surface, vk::PhysicalDevice physicalDev) noexcept {
        VulkanSwapChainSupportInfo supportInfo;
        supportInfo.Capabilities = physicalDev.getSurfaceCapabilitiesKHR(surface);
        supportInfo.Formats = physicalDev.getSurfaceFormatsKHR(surface);
        supportInfo.PresentModes = physicalDev.getSurfacePresentModesKHR(surface);
        return supportInfo;
    }

    VulkanSwapChainSupportInfo VulkanDevice::QuerySwapChainSupport(vk::SurfaceKHR surface) noexcept {
        return QuerySwapChainSupport(surface, m_physicalDevice);
    }


    bool VulkanDevice::DetectDepthFormat() noexcept {
        constexpr const std::array candidateFormats{
            vk::Format::eD32Sfloat,
            vk::Format::eD32SfloatS8Uint,
            vk::Format::eD24UnormS8Uint
        };

        auto&& flags = vk::FormatFeatureFlagBits::eDepthStencilAttachment;
        for(auto&& candidate : candidateFormats){

            auto&& formatProperties = m_physicalDevice.getFormatProperties(candidate); 

            if((formatProperties.linearTilingFeatures & flags) == flags){
                m_depthBufferFormat = candidate;
                return true;
            }
            else if((formatProperties.optimalTilingFeatures & flags) == flags) {
                m_depthBufferFormat = candidate;
                return true;
            }
        }

        return false;
    }

    void VulkanDevice::ReQuerySwapChainSupport(vk::SurfaceKHR surface) noexcept {
        m_swapChainInfo = this->QuerySwapChainSupport(surface);
    }

    void VulkanDevice::initDevice() noexcept{
        VulkanUtils::LogWarning("Gonna Select Physical Dev");
        VulkanUtils::LogWarning(std::format("Gonna Select Physical Dev: {} - {}", 1, 2));
        // std::clog << "Gonna Select Physical Dev\n";
        this->selectPhysicalDevice();
        
        this->createLogicalDevice();
            
        std::clog << "Queue Obta\n";

        this->createCommandPool();
    }

    bool VulkanDevice::selectQueueFamily(VulkanPhysicalDeviceRequirements& requirements, vk::PhysicalDevice physicalDev) noexcept
    {
        std::clog << "Select Queue\n";
        auto&& physicDevPro = physicalDev.getProperties();

            if(requirements.DiscreteGPU){
                if(physicDevPro.deviceType != vk::PhysicalDeviceType::eDiscreteGpu){
                    std::clog << "Loh GPU\n";
                        return false;
                }
            }

            {
                auto&& queuesPro = physicalDev.getQueueFamilyProperties();

                // We choose queue have small transfer score -> transfer queue
                uint32_t transferScore{255};
                for(int i{}; auto&& queue : queuesPro) {
                    uint32_t currentTransferScore{};

                    if(queue.queueFlags & vk::QueueFlagBits::eGraphics){
                        m_graphicsFamilyIndex = i;
                        ++currentTransferScore;
                    }

                    if(queue.queueFlags & vk::QueueFlagBits::eCompute){
                        m_computeFamilyIndex = i;
                        ++currentTransferScore;
                    }

                    if(queue.queueFlags & vk::QueueFlagBits::eTransfer){
                        if(currentTransferScore <= transferScore){
                            transferScore = currentTransferScore;
                            m_transferFamilyIndex = i;
                        }
                    }

                    // Check Support Present
                    if(physicalDev.getSurfaceSupportKHR(i, VulkanContext::GetInstance()->GetSurface())){
                        std::clog << "Queue " << i << " support present\n";
                        m_presentFamilyIndex = i;
                    }

                    ++i;
                }
            }

            // Juice
            if(
                (!requirements.Graphics || (requirements.Graphics && m_graphicsFamilyIndex != -1)) && 
                (!requirements.Present || (requirements.Present && m_presentFamilyIndex != -1)) &&
                (!requirements.Compute || (requirements.Compute && m_computeFamilyIndex != -1)) && 
                (!requirements.Transfer || (requirements.Transfer && m_transferFamilyIndex != -1))
            ){
                std::clog << "Device Met Require: " << physicDevPro.deviceName.data() << '\n';
                std::clog << "Graphic Index: " << m_graphicsFamilyIndex << '\n';
                std::clog << "Present Index: " << m_presentFamilyIndex << '\n';
                std::clog << "Compute Index: " << m_computeFamilyIndex << '\n';
                std::clog << "Transfer Index: " << m_transferFamilyIndex << '\n';

                m_swapChainInfo = QuerySwapChainSupport(VulkanContext::GetInstance()->GetSurface(), physicalDev);

                if(m_swapChainInfo.Formats.size() < 1 || m_swapChainInfo.PresentModes.size() < 1){
                    std::clog << "Swapchain not support skip dev\n";
                    return false;
                }

                if(requirements.DeviceExtensionName.size() > 0){

                    auto&& extensions = physicalDev.enumerateDeviceExtensionProperties();

                    for(auto&& requireEx : requirements.DeviceExtensionName){
                        bool isFound{false};
                        for(auto&& ex : extensions){

                            if(std::strcmp(ex.extensionName.data(), requireEx)){
                                isFound = true;
                            }
                        }

                        if(!isFound){
                            std::clog << "Extension " << requireEx << " not found!!!\n";
                            return false;
                        }
                    }
                }

                
                if(auto&& features = physicalDev.getFeatures(); requirements.SamplerAnisotropy && !features.samplerAnisotropy){
                    std::clog << "Device not support SamplerAnisotropy\n";
                }

                return true;
            }

        return false;
    }

    void VulkanDevice::selectPhysicalDevice() noexcept
    {
        std::clog << "Get Context Instance\n";
        
        auto&& totalPhysicDev = VulkanContext::GetInstance()->GetVulkanInstance().enumeratePhysicalDevices();
        if(totalPhysicDev.size() == 0){
            std::clog << "No Physical Device Support\n";
            return;
        }

        std::clog << "Total physicdev: " << totalPhysicDev.size() << '\n';

        for(auto&& physicDev : totalPhysicDev){
            VulkanPhysicalDeviceRequirements requirements{};
            requirements.Graphics = true;
            requirements.Present = true;
            requirements.Compute = true;
            requirements.Transfer = true;
            requirements.SamplerAnisotropy = true;
            requirements.DiscreteGPU = true;
            requirements.DeviceExtensionName.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            
            if(selectQueueFamily(requirements, physicDev)) {
                m_physicalDevice = physicDev;
                auto&& pro = m_physicalDevice.getProperties();
                std::clog << "Selected Device: " << pro.deviceName.data() << '\n';
                std::clog << "Type Device: " << std::to_underlying(pro.deviceType) << '\n';
                std::clog << "Graphic Index: " << m_graphicsFamilyIndex << '\n';
                std::clog << "Present Index: " << m_presentFamilyIndex << '\n';
                std::clog << "Compute Index: " << m_computeFamilyIndex << '\n';
                std::clog << "Transfer Index: " << m_transferFamilyIndex << '\n';
                break;
            }
        }

    }

    void VulkanDevice::createLogicalDevice() noexcept
    {
        std::clog << "Gonna Create Logical Dev\n";

        std::vector<int> indices;
        indices.emplace_back(m_graphicsFamilyIndex);
            
        if(!(m_graphicsFamilyIndex == m_presentFamilyIndex)){
            indices.emplace_back(m_presentFamilyIndex);
        }

        if(!(m_graphicsFamilyIndex == m_transferFamilyIndex)){
            indices.emplace_back(m_transferFamilyIndex);
        }

        std::vector<vk::DeviceQueueCreateInfo> deviceQueueInfos;
        deviceQueueInfos.reserve(indices.size());

        for(size_t i{}; i < indices.size(); ++i){
            vk::DeviceQueueCreateInfo info{};
            info.flags = vk::DeviceQueueCreateFlags{};
            info.queueFamilyIndex = indices.at(i);
            info.queueCount = 1;
            info.pNext = nullptr;
            constexpr float priorities{1.0f};
            info.pQueuePriorities = &priorities;
            deviceQueueInfos.emplace_back(info);
        }

        vk::PhysicalDeviceFeatures physicalDevFeatures{};
        physicalDevFeatures.samplerAnisotropy = vk::True;
            
        constexpr std::array<const char*, 1> extensions{
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        vk::DeviceCreateInfo deviceInfo{};
        deviceInfo.flags = vk::DeviceCreateFlags{};
        deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(deviceQueueInfos.size());
        deviceInfo.pQueueCreateInfos = deviceQueueInfos.data();
        deviceInfo.pEnabledFeatures = &physicalDevFeatures;
        deviceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        deviceInfo.ppEnabledExtensionNames = extensions.data();

        std::clog << "Dev Size: " << deviceQueueInfos.size() << ", Ex C: " << extensions.size() << '\n';
            
        VULKAN_CHECK_ERROR(m_logicalDevice = m_physicalDevice.createDevice(deviceInfo));

        std::clog << "Created Logical Dev\n";

        // Select Queue
        m_graphicsQueue = m_logicalDevice.getQueue(m_graphicsFamilyIndex, 0);

        m_presentQueue = m_logicalDevice.getQueue(m_presentFamilyIndex, 0);
            
        m_transferQueue = m_logicalDevice.getQueue(m_transferFamilyIndex, 0);
    }

    void VulkanDevice::createCommandPool() noexcept
    {
        vk::CommandPoolCreateInfo commandPoolInfo{};
        commandPoolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        commandPoolInfo.queueFamilyIndex = m_graphicsFamilyIndex;

        VULKAN_CHECK_ERROR(GraphicCommandPool = m_logicalDevice.createCommandPool(commandPoolInfo));
    }
}