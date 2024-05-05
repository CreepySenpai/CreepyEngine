#pragma once

#include <vector>
#include <memory>
#include <span>
#include <vulkan/vulkan.hpp>
#include "VulkanFrameBuffer.hpp"
#include "VulkanTypes.hpp"

namespace Creepy {

    class VulkanImage;

    struct VulkanSwapChainSpec;

    class VulkanSwapChain{
        public:
            VulkanSwapChain(const VulkanSwapChainSpec& swapChainSpec) noexcept;

            void Recreate(const VulkanSwapChainSpec& swapChainSpec) noexcept;
            void Destroy(const vk::Device logicalDev) noexcept;
            
            uint32_t AcquireNextImageIndex(uint64_t timeOut, const vk::Semaphore imgAvailableSemaphore, const vk::Fence fence) noexcept;

            vk::Result Present(const vk::Queue presentQueue, const vk::Semaphore renderComplete, uint32_t presentImageIndex) noexcept;

            constexpr vk::SwapchainKHR GetHandle() const noexcept {
                return m_handle;
            }

            const vk::SurfaceFormatKHR& GetImageFormat() const noexcept {
                return m_imageFormat;
            }

            std::span<const vk::Image> GetImages() const noexcept {
                return m_images;
            }

            std::span<const vk::ImageView> GetImageViews() const noexcept {
                return m_imageViews;
            }

            constexpr uint32_t GetMaxFramesInFlight() const noexcept {
                return m_maxFramesInFlight;
            }

            std::vector<VulkanFrameBuffer>& GetFrameBuffers() noexcept {
                return m_frameBuffers;
            }

            std::shared_ptr<VulkanImage> GetDepthBuffer() const noexcept;
        
        private:
            void createSwapChain(const VulkanSwapChainSpec& swapChainSpec) noexcept;
            void createHandle(const VulkanSwapChainSpec& swapChainSpec, vk::PresentModeKHR presentMode) noexcept;
            void createDepthBuffer(const vk::Device logicalDev, uint32_t width, uint32_t height, vk::Format depthFormat) noexcept;
        private:
            vk::SurfaceFormatKHR m_imageFormat;
            uint32_t m_maxFramesInFlight{};
            vk::SwapchainKHR m_handle{nullptr};
            std::vector<vk::Image> m_images;
            std::vector<vk::ImageView> m_imageViews;
            std::vector<VulkanFrameBuffer> m_frameBuffers;
            std::shared_ptr<VulkanImage> m_depthBuffer{nullptr};
    };

}