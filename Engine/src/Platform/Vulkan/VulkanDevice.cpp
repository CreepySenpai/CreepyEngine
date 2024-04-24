#include <utility>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>

namespace Creepy{

    struct VulkanPhysicalDeviceRequirements{
        bool Graphics{false};
        bool Present{false};
        bool Compute{false};
        bool Transfer{false};
        bool SamplerAnisotropy{false};
        bool DiscreteGPU{false};
        std::vector<const char*> DeviceExtensionName;
    };

    struct VulkanPhysicalDeviceQueueFamilyInfo{
        int GraphicsFamilyIndex{-1};
        int PresentFamilyIndex{-1};
        int ComputeFamilyIndex{-1};
        int TransferFamilyIndex{-1};
    };

    // Forward Declare
    bool selectQueueFamily(VulkanContext* context, vk::PhysicalDevice& physicDev, const VulkanPhysicalDeviceRequirements& requirements, VulkanPhysicalDeviceQueueFamilyInfo& queueInfo, VulkanSwapChainSupportInfo& swapChainInfo) noexcept;
    
    bool selectPhysicalDevice(VulkanContext* context) noexcept;

    bool Device::CreateVulkanDevice(VulkanContext* context) noexcept {

        std::clog << "Gonna Select Physical Dev\n";

        if(selectPhysicalDevice(context)){
            std::clog << "Gonna Create Logical Dev\n";

            std::vector<int> indices;
            indices.emplace_back(context->Devices.GraphicsFamilyIndex);
            
            if(!(context->Devices.GraphicsFamilyIndex == context->Devices.PresentFamilyIndex)){
                indices.emplace_back(context->Devices.PresentFamilyIndex);
            }

            if(!(context->Devices.GraphicsFamilyIndex == context->Devices.TransferFamilyIndex)){
                indices.emplace_back(context->Devices.TransferFamilyIndex);
            }

            std::vector<vk::DeviceQueueCreateInfo> deviceQueueInfos;
            deviceQueueInfos.reserve(indices.size());

            for(size_t i{}; i < indices.size(); ++i){
                vk::DeviceQueueCreateInfo info{};
                info.flags = vk::DeviceQueueCreateFlags{};
                info.queueFamilyIndex = indices.at(i);
                info.queueCount = 1;
                // info.queueCount = indices.at(i) == context->Devices.GraphicsFamilyIndex ? 2 : 1;
                info.pNext = nullptr;
                float priorities{1.0f};
                info.pQueuePriorities = &priorities;
                deviceQueueInfos.emplace_back(info);
            }

            vk::PhysicalDeviceFeatures physicalDevFeatures{};
            physicalDevFeatures.samplerAnisotropy = vk::True;
            
            std::array<const char*, 1> extensions{
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
            
            VULKAN_CHECK_ERROR(context->Devices.LogicalDevice = context->Devices.PhysicalDevice.createDevice(deviceInfo));

            std::clog << "Created Logical Dev\n";

            context->Devices.GraphicsQueue = context->Devices.LogicalDevice.getQueue(context->Devices.GraphicsFamilyIndex, 0);

            context->Devices.PresentQueue = context->Devices.LogicalDevice.getQueue(context->Devices.PresentFamilyIndex, 0);
            
            context->Devices.TransferQueue = context->Devices.LogicalDevice.getQueue(context->Devices.TransferFamilyIndex, 0);
            
            std::clog << "Queue Obta\n";

            return true;
        }
        return false;
    }

    void Device::DestroyDevice(VulkanContext* context) noexcept {
        context->Devices.LogicalDevice.destroy();
    }

    void Device::VulkanDeviceQuerySwapChainSupport(vk::PhysicalDevice physicDev, vk::SurfaceKHR surface, VulkanSwapChainSupportInfo& info) noexcept {
        info.Capabilities = physicDev.getSurfaceCapabilitiesKHR(surface);
        info.Formats = physicDev.getSurfaceFormatsKHR(surface);
        info.PresentModes = physicDev.getSurfacePresentModesKHR(surface);

    }

    bool Device::VulkanDeviceDetectDepthFormat(VulkanDevice& devices) noexcept {
        constexpr const std::array candidateFormats{
            vk::Format::eD32Sfloat,
            vk::Format::eD32SfloatS8Uint,
            vk::Format::eD24UnormS8Uint
        };

        auto&& flags = vk::FormatFeatureFlagBits::eDepthStencilAttachment;
        for(auto&& candidate : candidateFormats){

            auto&& formatProperties = devices.PhysicalDevice.getFormatProperties(candidate); 

            if((formatProperties.linearTilingFeatures & flags) == flags){
                devices.DepthFormat = candidate;
                return true;
            }
            else if((formatProperties.optimalTilingFeatures & flags) == flags) {
                devices.DepthFormat = candidate;
                return true;
            }
        }

        return false;
    }

    bool selectQueueFamily(VulkanContext* context, vk::PhysicalDevice& physicDev, const VulkanPhysicalDeviceRequirements& requirements, VulkanPhysicalDeviceQueueFamilyInfo& queueInfo, VulkanSwapChainSupportInfo& swapChainInfo) noexcept {
            auto&& physicDevPro = physicDev.getProperties();

            if(requirements.DiscreteGPU){
                if(physicDevPro.deviceType != vk::PhysicalDeviceType::eDiscreteGpu){
                    std::clog << "Loh GPU\n";
                        return false;
                }
            }

            {
                auto&& queuesPro = physicDev.getQueueFamilyProperties();

                // We choose queue have small transfer score -> transfer queue
                uint32_t transferScore{255};
                for(int i{}; auto&& queue : queuesPro) {
                    uint32_t currentTransferScore{};

                    if(queue.queueFlags & vk::QueueFlagBits::eGraphics){
                        queueInfo.GraphicsFamilyIndex = i;
                        ++currentTransferScore;
                    }

                    if(queue.queueFlags & vk::QueueFlagBits::eCompute){
                        queueInfo.ComputeFamilyIndex = i;
                        ++currentTransferScore;
                    }

                    if(queue.queueFlags & vk::QueueFlagBits::eTransfer){
                        if(currentTransferScore <= transferScore){
                            transferScore = currentTransferScore;
                            queueInfo.TransferFamilyIndex = i;
                        }
                    }

                    // Check Support Present
                    if(physicDev.getSurfaceSupportKHR(i, context->Surface)){
                        std::clog << "Queue " << i << " support present\n";
                        queueInfo.PresentFamilyIndex = i;
                    }

                    ++i;
                }
            }

            {
                std::clog << "Device Name: " << physicDevPro.deviceName.data() << '\n';
                std::clog << "Graphic Index: " << queueInfo.GraphicsFamilyIndex << '\n';
                std::clog << "Present Index: " << queueInfo.PresentFamilyIndex << '\n';
                std::clog << "Compute Index: " << queueInfo.ComputeFamilyIndex << '\n';
                std::clog << "Transfer Index: " << queueInfo.TransferFamilyIndex << '\n';
            }

            // Juice
            if(
                (!requirements.Graphics || (requirements.Graphics && queueInfo.GraphicsFamilyIndex != -1)) && 
                (!requirements.Present || (requirements.Present && queueInfo.PresentFamilyIndex != -1)) &&
                (!requirements.Compute || (requirements.Compute && queueInfo.ComputeFamilyIndex != -1)) && 
                (!requirements.Transfer || (requirements.Transfer && queueInfo.TransferFamilyIndex != -1))
            ){
                std::clog << "Device Met Require: " << physicDevPro.deviceName.data() << '\n';
                std::clog << "Graphic Index: " << queueInfo.GraphicsFamilyIndex << '\n';
                std::clog << "Present Index: " << queueInfo.PresentFamilyIndex << '\n';
                std::clog << "Compute Index: " << queueInfo.ComputeFamilyIndex << '\n';
                std::clog << "Transfer Index: " << queueInfo.TransferFamilyIndex << '\n';

                Device::VulkanDeviceQuerySwapChainSupport(physicDev, context->Surface, swapChainInfo);

                if(swapChainInfo.Formats.size() < 1 || swapChainInfo.PresentModes.size() < 1){
                    std::clog << "Swapchain not support skip dev\n";
                    return false;
                }

                if(requirements.DeviceExtensionName.size() > 0){

                    auto&& extensions = physicDev.enumerateDeviceExtensionProperties();

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

                
                if(auto&& features = physicDev.getFeatures(); requirements.SamplerAnisotropy && !features.samplerAnisotropy){
                    std::clog << "Device not support SamplerAnisotropy\n";
                }

                return true;
            }

        return false;
    }

    bool selectPhysicalDevice(VulkanContext* context) noexcept {
        auto&& totalPhysicDev = context->Instance.enumeratePhysicalDevices();

        if(totalPhysicDev.size() == 0){
            std::clog << "No Physical Device Support\n";
            return false;
        }

        for(auto&& physicDev : totalPhysicDev){
            VulkanPhysicalDeviceRequirements requirements{};
            requirements.Graphics = true;
            requirements.Present = true;
            requirements.Compute = true;
            requirements.Transfer = true;
            requirements.SamplerAnisotropy = true;
            requirements.DiscreteGPU = true;
            requirements.DeviceExtensionName.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

            VulkanPhysicalDeviceQueueFamilyInfo queueInfo{};

            if(selectQueueFamily(context, physicDev, requirements, queueInfo, context->Devices.SwapChainSupport)) {
                auto&& pro = physicDev.getProperties();
                std::clog << "Selected Device: " << pro.deviceName.data() << '\n';
                std::clog << "Type Device: " << std::to_underlying(pro.deviceType) << '\n';
                
                context->Devices.PhysicalDevice = physicDev;
                context->Devices.GraphicsFamilyIndex = queueInfo.GraphicsFamilyIndex;
                context->Devices.PresentFamilyIndex = queueInfo.PresentFamilyIndex;
                context->Devices.TransferFamilyIndex = queueInfo.TransferFamilyIndex;

                context->Devices.PhysicalDeviceProperties = pro;
                context->Devices.PhysicalDeviceFeatures = physicDev.getFeatures();
                context->Devices.PhysicalDeviceMemory = physicDev.getMemoryProperties();
                break;
            }
        }

        return true;
    }

}