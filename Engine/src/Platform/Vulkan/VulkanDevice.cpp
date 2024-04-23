#include <CreepyEngine/Core/Core.hpp>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>

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
        return selectPhysicalDevice(context);
    }

    void Device::DestroyDevice(VulkanContext* context) noexcept {
    
    }

    void Device::VulkanDeviceQuerySwapChainSupport(vk::PhysicalDevice physicDev, vk::SurfaceKHR surface, VulkanSwapChainSupportInfo& info) noexcept {
        info.Capabilities = physicDev.getSurfaceCapabilitiesKHR(surface);
        info.Formats = physicDev.getSurfaceFormatsKHR(surface);
        info.PresentModes = physicDev.getSurfacePresentModesKHR(surface);

    }

    bool selectQueueFamily(VulkanContext* context, vk::PhysicalDevice& physicDev, const VulkanPhysicalDeviceRequirements& requirements, VulkanPhysicalDeviceQueueFamilyInfo& queueInfo, VulkanSwapChainSupportInfo& swapChainInfo) noexcept {
            auto&& physicDevPro = physicDev.getProperties();

            if(requirements.DiscreteGPU){
                if(physicDevPro.deviceType != vk::PhysicalDeviceType::eDiscreteGpu){
                    ENGINE_LOG_ERROR("Loh GPU");
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
                        ENGINE_LOG_WARNING("Queue {} support present", i);
                        queueInfo.PresentFamilyIndex = i;
                    }

                    ++i;
                }
            }

            {
                ENGINE_LOG_WARNING("Device Name: {}", std::string{physicDevPro.deviceName.data()});
                ENGINE_LOG_WARNING("Graphic Index: {}", queueInfo.GraphicsFamilyIndex);
                ENGINE_LOG_WARNING("Present Index: {}", queueInfo.PresentFamilyIndex);
                ENGINE_LOG_WARNING("Compute Index: {}", queueInfo.ComputeFamilyIndex);
                ENGINE_LOG_WARNING("Transfer Index: {}", queueInfo.TransferFamilyIndex);
            }

            // Juice
            if(
                (!requirements.Graphics || (requirements.Graphics && queueInfo.GraphicsFamilyIndex != -1)) && 
                (!requirements.Present || (requirements.Present && queueInfo.PresentFamilyIndex != -1)) &&
                (!requirements.Compute || (requirements.Compute && queueInfo.ComputeFamilyIndex != -1)) && 
                (!requirements.Transfer || (requirements.Transfer && queueInfo.TransferFamilyIndex != -1))
            ){
                ENGINE_LOG_WARNING("Device Met Require: {}", std::string{physicDevPro.deviceName.data()});
                ENGINE_LOG_WARNING("Graphic Index: {}", queueInfo.GraphicsFamilyIndex);
                ENGINE_LOG_WARNING("Present Index: {}", queueInfo.PresentFamilyIndex);
                ENGINE_LOG_WARNING("Compute Index: {}", queueInfo.ComputeFamilyIndex);
                ENGINE_LOG_WARNING("Transfer Index: {}", queueInfo.TransferFamilyIndex);

                Device::VulkanDeviceQuerySwapChainSupport(physicDev, context->Surface, swapChainInfo);

                if(swapChainInfo.Formats.size() < 1 || swapChainInfo.PresentModes.size() < 1){
                    ENGINE_LOG_WARNING("Swapchain not support skip dev");
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
                            ENGINE_LOG_ERROR("Extension {} not found!!!", std::string{requireEx});
                            return false;
                        }
                    }
                }

                
                if(auto&& features = physicDev.getFeatures(); requirements.SamplerAnisotropy && !features.samplerAnisotropy){
                    ENGINE_LOG_ERROR("Device not support SamplerAnisotropy");
                }

                return true;
            }

        return false;
    }

    bool selectPhysicalDevice(VulkanContext* context) noexcept {
        auto&& totalPhysicDev = context->Instance.enumeratePhysicalDevices();

        if(totalPhysicDev.size() == 0){
            ENGINE_LOG_ERROR("No Physical Device Support");
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
                ENGINE_LOG_WARNING("Selected Device: {}", std::string{pro.deviceName.data()});
                ENGINE_LOG_WARNING("Type Device: {}", std::to_underlying(pro.deviceType));
                
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