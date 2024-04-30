#include <algorithm>
#include <iostream>
#include <Platform/Vulkan/VulkanSwapChain.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <Platform/Vulkan/VulkanImage.hpp>
#include <Platform/Vulkan/VulkanTypes.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>

namespace Creepy{

    VulkanSwapChain::VulkanSwapChain(const VulkanSwapChainSpec& swapChainSpec) noexcept : m_maxFramesInFlight{2} {
        std::clog << "Create Swapchain 1\n";
        createSwapChain(swapChainSpec);
    }

    void VulkanSwapChain::Recreate(const VulkanSwapChainSpec& swapChainSpec) noexcept {
        this->Destroy(swapChainSpec.LogicalDev);
        createSwapChain(swapChainSpec);
    }

    void VulkanSwapChain::Destroy(vk::Device logicalDev) noexcept {
        
        m_depthBuffer->Destroy(logicalDev);

        m_images.clear();

        for(auto&& view : m_imageViews){
            logicalDev.destroyImageView(view);
        }

        m_imageViews.clear();

        std::clog << "Destroy Sw\n";

        logicalDev.destroySwapchainKHR(m_handle);
        
        m_handle = nullptr;
    }

    uint32_t VulkanSwapChain::AcquireNextImageIndex(uint64_t timeOut, vk::Semaphore imgAvailableSemaphore, vk::Fence fence) noexcept {
        return VulkanContext::GetInstance()->GetLogicalDevice().acquireNextImageKHR(m_handle, timeOut, imgAvailableSemaphore, fence).value;
    }
    
    vk::Result VulkanSwapChain::Present(vk::Queue presentQueue, vk::Semaphore renderComplete, uint32_t presentImageIndex) noexcept {
        vk::PresentInfoKHR presentInfo{};
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &renderComplete;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &m_handle;
        presentInfo.pImageIndices = &presentImageIndex;

        return presentQueue.presentKHR(presentInfo);
    }


    void VulkanSwapChain::createSwapChain(const VulkanSwapChainSpec& swapChainSpec) noexcept {

        bool isFound{false};

        auto&& swapChainInfo =  VulkanContext::GetInstance()->GetDevices()->GetSwapChainSupportInfo();

        for(auto&& supportFormat : swapChainInfo.Formats){
            
            if(supportFormat.format == vk::Format::eB8G8R8A8Unorm && supportFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear){
                m_imageFormat = supportFormat.format;
                isFound = true;
                break;
            }
        }

        if(!isFound){
            m_imageFormat = swapChainInfo.Formats.at(0);
        }

        vk::PresentModeKHR presentMode{vk::PresentModeKHR::eFifo};
        for(auto&& supportsPresent : swapChainInfo.PresentModes){
            if(supportsPresent == vk::PresentModeKHR::eMailbox){
                presentMode = supportsPresent;
                break;
            }
        }

        std::clog << "ChainSupportInfo\n";

        swapChainInfo = VulkanContext::GetInstance()->GetDevices()->QuerySwapChainSupport(VulkanContext::GetInstance()->GetSurface());

        std::clog << "createSwapChain\n";

        createHandle(swapChainSpec, presentMode);
    }

    void VulkanSwapChain::createHandle(const VulkanSwapChainSpec& swapChainSpec, vk::PresentModeKHR presentMode) noexcept
    {
        
        vk::Extent2D swapChainExtent{swapChainSpec.Width, swapChainSpec.Height};
        auto&& swapChainSupportInfo =  VulkanContext::GetInstance()->GetDevices()->GetSwapChainSupportInfo();
        auto &&capabilities = swapChainSupportInfo.Capabilities;
        if (capabilities.currentExtent != std::numeric_limits<uint32_t>::max())
        {
            swapChainExtent = capabilities.currentExtent;
        }

        auto &&minExt = capabilities.minImageExtent;
        auto &&maxExt = capabilities.maxImageExtent;
        swapChainExtent.width = std::clamp(swapChainExtent.width, minExt.width, maxExt.width);
        swapChainExtent.height = std::clamp(swapChainExtent.height, minExt.height, maxExt.height);

        auto &&imageCount{capabilities.minImageCount + 1};
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
        {
            imageCount = capabilities.maxImageCount;
        }

        vk::SwapchainCreateInfoKHR swapChainInfo{};
        swapChainInfo.flags = vk::SwapchainCreateFlagsKHR{};
        swapChainInfo.surface = swapChainSpec.Surface;
        swapChainInfo.minImageCount = imageCount;
        swapChainInfo.imageFormat = m_imageFormat.format;
        swapChainInfo.imageColorSpace = m_imageFormat.colorSpace;
        swapChainInfo.imageExtent = swapChainExtent;
        swapChainInfo.imageArrayLayers = 1;
        swapChainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        swapChainInfo.oldSwapchain = nullptr;

        if (swapChainSpec.GraphicFamilyIndex != swapChainSpec.PresentFamilyIndex)
        {
            const std::array<uint32_t, 2> queueFamilyIndices{
                static_cast<uint32_t>(swapChainSpec.GraphicFamilyIndex),
                static_cast<uint32_t>(swapChainSpec.PresentFamilyIndex)};

            std::clog << "Choose Mode Concurency\n";
            swapChainInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            swapChainInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
            swapChainInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        }
        else
        {
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

        
        VULKAN_CHECK_ERROR(m_handle = swapChainSpec.LogicalDev.createSwapchainKHR(swapChainInfo));

        std::clog << "Created Swapchain\n";

        VulkanContext::GetInstance()->SetCurrentFrame(0);

        VULKAN_CHECK_ERROR(m_images = swapChainSpec.LogicalDev.getSwapchainImagesKHR(m_handle));

        std::clog << "Total Image: " << m_images.size() << '\n';

        m_maxFramesInFlight = static_cast<int>(m_images.size());

        for (auto &&image : m_images)
        {
            vk::ImageViewCreateInfo imgViewInfo{};
            imgViewInfo.flags = vk::ImageViewCreateFlags{};
            imgViewInfo.viewType = vk::ImageViewType::e2D;
            imgViewInfo.image = image;
            imgViewInfo.format = m_imageFormat.format;
            imgViewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            imgViewInfo.subresourceRange.baseMipLevel = 0;
            imgViewInfo.subresourceRange.levelCount = 1;
            imgViewInfo.subresourceRange.baseArrayLayer = 0;
            imgViewInfo.subresourceRange.layerCount = 1;

            VULKAN_CHECK_ERROR(m_imageViews.emplace_back(swapChainSpec.LogicalDev.createImageView(imgViewInfo)));
        }

        std::clog << "SwapChain Create Success\n";
        
        auto&& devicesInstance = VulkanContext::GetInstance()->GetDevices();
        if(devicesInstance->DetectDepthFormat()){
            std::clog << "Has Detect Depth Format\n";
        }
        else {
            devicesInstance->SetDepthBufferFormat(vk::Format::eUndefined);
            std::clog << "Cannot Detect Depth Format\n";
        }

        createDepthBuffer(swapChainSpec.LogicalDev, swapChainExtent.width, swapChainExtent.height, devicesInstance->GetDepthBufferFormat());
    }

    void VulkanSwapChain::createDepthBuffer(vk::Device logicalDev, uint32_t width, uint32_t height, vk::Format depthFormat) noexcept {
        
        VulkanImageSpec imgSpec;
        imgSpec.ImageType = vk::ImageType::e2D;
        imgSpec.Width = width;
        imgSpec.Height = height;
        imgSpec.Format = depthFormat;
        imgSpec.Tiling = vk::ImageTiling::eOptimal;
        imgSpec.Usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
        imgSpec.MemoryFlags = vk::MemoryPropertyFlagBits::eDeviceLocal;
        imgSpec.IsCreateView = true;
        imgSpec.Aspect = vk::ImageAspectFlagBits::eDepth;
        imgSpec.LogicalDev = logicalDev;

        m_depthBuffer = std::make_shared<VulkanImage>(imgSpec);

    }

    std::shared_ptr<VulkanImage> VulkanSwapChain::GetDepthBuffer() const noexcept {
        return m_depthBuffer;
    }

}