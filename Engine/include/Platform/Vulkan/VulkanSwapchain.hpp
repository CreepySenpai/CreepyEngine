#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>
#include "VulkanImage.hpp"

namespace Creepy{

    class VulkanContext;

    struct VulkanSwapchain{

        vk::SurfaceFormatKHR ImageFormat;
        uint32_t MaxFramesInFlight{};
        vk::SwapchainKHR SwapchainHandle{nullptr};
        std::vector<vk::Image> Images;
        std::vector<vk::ImageView> ImageViews;
        VulkanImage DepthAttachment{};

        static void CreateSwapChain(VulkanContext* context, uint32_t width, uint32_t height, VulkanSwapchain& swapChain) noexcept;

        static void RecreateSwapChain(VulkanContext* context, uint32_t width, uint32_t height, VulkanSwapchain& swapChain) noexcept;

        static void DestroySwapChain(VulkanContext* context, VulkanSwapchain& swapChain) noexcept;

        static uint32_t SwapChainAcquireNextImageIndex(VulkanContext* context, VulkanSwapchain& swapChain, uint64_t timeOut, vk::Semaphore imgAvailableSemaphore, vk::Fence fence) noexcept;

        static void SwapChainPresent(VulkanContext* context, VulkanSwapchain& swapChain, vk::Queue graphicQueue, vk::Queue presentQueue, vk::Semaphore renderComplete, uint32_t presentImageIndex) noexcept;
    };
}