#include <limits>
#include <Platform/Vulkan/VulkanSwapchain.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <Platform/Vulkan/VulkanImage.hpp>

namespace Creepy {

    static void createSwapChain(VulkanContext* context, uint32_t width, uint32_t height, VulkanSwapchain& swapChain) noexcept {
        swapChain.MaxFramesInFlight = 2;

        bool isFound{false};
        for(auto&& supportFormat : context->Devices.SwapChainSupport.Formats){
            
            if(supportFormat.format == vk::Format::eB8G8R8A8Unorm && supportFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear){
                swapChain.ImageFormat = supportFormat.format;
                isFound = true;
                break;
            }
        }

        if(!isFound){
            swapChain.ImageFormat = context->Devices.SwapChainSupport.Formats[0];
        }

        vk::PresentModeKHR presentMode{vk::PresentModeKHR::eFifo};
        for(auto&& supportsPresent : context->Devices.SwapChainSupport.PresentModes){
            if(supportsPresent == vk::PresentModeKHR::eMailbox){
                presentMode = supportsPresent;
                break;
            }
        }

        Device::VulkanDeviceQuerySwapChainSupport(context->Devices.PhysicalDevice, context->Surface, context->Devices.SwapChainSupport);

        {
            vk::Extent2D swapChainExtent{width, height};
            auto&& capabilities = context->Devices.SwapChainSupport.Capabilities;
            if(capabilities.currentExtent != std::numeric_limits<uint32_t>::max()){
                swapChainExtent = capabilities.currentExtent;
            }

            auto&& minExt = capabilities.minImageExtent;
            auto&& maxExt = capabilities.maxImageExtent;
            swapChainExtent.width = std::clamp(swapChainExtent.width, minExt.width, maxExt.width);
            swapChainExtent.height = std::clamp(swapChainExtent.height, minExt.height, maxExt.height);

            auto&& imageCount{capabilities.minImageCount + 1};
            if(capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount){
                imageCount = capabilities.maxImageCount;
            }

            vk::SwapchainCreateInfoKHR swapChainInfo{};
            swapChainInfo.flags = vk::SwapchainCreateFlagsKHR{};
            swapChainInfo.surface = context->Surface;
            swapChainInfo.minImageCount = imageCount;
            swapChainInfo.imageFormat = swapChain.ImageFormat.format;
            swapChainInfo.imageColorSpace = swapChain.ImageFormat.colorSpace;
            swapChainInfo.imageExtent = swapChainExtent;
            swapChainInfo.imageArrayLayers = 1;
            swapChainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
            swapChainInfo.oldSwapchain = nullptr;


            if(context->Devices.GraphicsFamilyIndex != context->Devices.PresentFamilyIndex){
                
                std::array<uint32_t, 2> queueFamilyIndices{
                    static_cast<uint32_t>(context->Devices.GraphicsFamilyIndex),
                    static_cast<uint32_t>(context->Devices.PresentFamilyIndex)
                };
                std::clog << "Choose Mode Concurency\n";
                swapChainInfo.imageSharingMode = vk::SharingMode::eConcurrent;
                swapChainInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
                swapChainInfo.pQueueFamilyIndices = queueFamilyIndices.data();
            }
            else {
                std::clog << "Choose Mode Exclusive\n";
                swapChainInfo.imageSharingMode = vk::SharingMode::eExclusive;
                swapChainInfo.queueFamilyIndexCount = 0;
                swapChainInfo.pQueueFamilyIndices = nullptr;
            }

            swapChainInfo.preTransform = capabilities.currentTransform;
            swapChainInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
            swapChainInfo.presentMode = presentMode;
            swapChainInfo.clipped = vk::True;

            std::clog << "Gonna Create Swapchain\n";

            VULKAN_CHECK_ERROR(swapChain.SwapchainHandle = context->Devices.LogicalDevice.createSwapchainKHR(swapChainInfo));

            std::clog << "Created Swapchain\n";

            context->CurrentFrame = 0;
            
            VULKAN_CHECK_ERROR(swapChain.Images = context->Devices.LogicalDevice.getSwapchainImagesKHR(swapChain.SwapchainHandle));

            std::clog << "Total Image: " << swapChain.Images.size() << '\n';
            for(auto&& image : swapChain.Images){

                vk::ImageViewCreateInfo imgViewInfo{};
                imgViewInfo.flags = vk::ImageViewCreateFlags{};
                imgViewInfo.viewType = vk::ImageViewType::e2D;
                imgViewInfo.image = image;
                imgViewInfo.format = swapChain.ImageFormat.format;
                imgViewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
                imgViewInfo.subresourceRange.baseMipLevel = 0;
                imgViewInfo.subresourceRange.levelCount = 1;
                imgViewInfo.subresourceRange.baseArrayLayer = 0;
                imgViewInfo.subresourceRange.layerCount = 1;

                VULKAN_CHECK_ERROR(swapChain.ImageViews.emplace_back(context->Devices.LogicalDevice.createImageView(imgViewInfo)));

            }

            if(!Device::VulkanDeviceDetectDepthFormat(context->Devices)){
                std::clog << "Cannot find supported flags\n";
                context->Devices.DepthFormat = vk::Format::eUndefined;
            }

            {
                VulkanImage::CreateImage(context, vk::ImageType::e2D, swapChainExtent.width, 
                swapChainExtent.height, context->Devices.DepthFormat, vk::ImageTiling::eOptimal,
                vk::ImageUsageFlagBits::eDepthStencilAttachment, 
                vk::MemoryPropertyFlagBits::eDeviceLocal, true, vk::ImageAspectFlagBits::eDepth, swapChain.DepthAttachment);
            }

        }

        std::clog << "Swapchain create success\n";

    }

    static void destroySwapChain(VulkanContext* context, VulkanSwapchain& swapChain) noexcept {
        VulkanImage::DestroyImage(context, swapChain.DepthAttachment);
        
        for(auto&& view : swapChain.ImageViews){
            context->Devices.LogicalDevice.destroyImageView(view);
        }

        context->Devices.LogicalDevice.destroySwapchainKHR(swapChain.SwapchainHandle);

    }

    void VulkanSwapchain::CreateSwapChain(VulkanContext* context, uint32_t width, uint32_t height, VulkanSwapchain& swapChain) noexcept {
        createSwapChain(context, width, height, swapChain);
    }

    void VulkanSwapchain::RecreateSwapChain(VulkanContext* context, uint32_t width, uint32_t height, VulkanSwapchain& swapChain) noexcept {
        destroySwapChain(context, swapChain);
        createSwapChain(context, width, height, swapChain);
    }

    void VulkanSwapchain::DestroySwapChain(VulkanContext* context, VulkanSwapchain& swapChain) noexcept {
        destroySwapChain(context, swapChain);
    }

    uint32_t VulkanSwapchain::SwapChainAcquireNextImageIndex(VulkanContext* context, VulkanSwapchain& swapChain, uint64_t timeOut, vk::Semaphore imgAvailableSemaphore, vk::Fence fence) noexcept {
        vk::Result res{};
        uint32_t val{};
        try{
            auto&& acquire = context->Devices.LogicalDevice.acquireNextImageKHR(swapChain.SwapchainHandle, timeOut, imgAvailableSemaphore, fence);
            res = acquire.result;
            val = acquire.value;
            return val;
        }
        catch(const vk::SystemError& e){

            std::clog << "Has Error: " << e.what() << '\n';

            if(res == vk::Result::eErrorOutOfDateKHR){
                std::clog << "Recreate Swap Chain\n";
                VulkanSwapchain::RecreateSwapChain(context, context->FrameBufferWidth, context->FrameBufferHeight, swapChain);
            }

            // TODO: More Error
            
        }
        std::clog << "Acquire Error\n";
        return val;
    }

    void VulkanSwapchain::SwapChainPresent(VulkanContext* context, VulkanSwapchain& swapChain, vk::Queue graphicQueue, vk::Queue presentQueue, vk::Semaphore renderComplete, uint32_t presentImageIndex) noexcept {
        vk::PresentInfoKHR presentInfo{};
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &renderComplete;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapChain.SwapchainHandle;
        presentInfo.pImageIndices = &presentImageIndex;

        auto&& res = graphicQueue.presentKHR(presentInfo);

        if(res == vk::Result::eErrorOutOfDateKHR || res == vk::Result::eSuboptimalKHR){
            std::clog << "Recreate Swap Chain In Present\n";
            VulkanSwapchain::RecreateSwapChain(context, context->FrameBufferWidth, context->FrameBufferHeight, swapChain);
        }

    }

}