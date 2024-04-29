#pragma once

#include <vulkan/vulkan.hpp>
#include "VulkanTypes.hpp"

namespace Creepy{

    class VulkanDevice{
        public:
            VulkanDevice() noexcept;
            
            void DestroyDevice() noexcept;

            VulkanSwapChainSupportInfo QuerySwapChainSupport(vk::SurfaceKHR surface, vk::PhysicalDevice physicalDev) noexcept;
            VulkanSwapChainSupportInfo QuerySwapChainSupport(vk::SurfaceKHR surface) noexcept;
            bool DetectDepthFormat() noexcept;

            vk::PhysicalDevice GetPhysicalDevice() const noexcept {
                return m_physicalDevice;
            }

            vk::Device GetLogicalDevice() const noexcept {
                return m_logicalDevice;
            }

            constexpr vk::Queue GetGraphicsQueue() const noexcept {
                return m_graphicsQueue;
            }

            constexpr vk::Queue GetPresentQueue() const noexcept {
                return m_presentQueue;
            }

            constexpr uint32_t GetGraphicsFamilyIndex() const noexcept {
                return m_graphicsFamilyIndex;
            }

            constexpr uint32_t GetComputeFamilyIndex() const noexcept {
                return m_computeFamilyIndex;
            }

            constexpr uint32_t GetPresentFamilyIndex() const noexcept {
                return m_presentFamilyIndex;
            }

            constexpr uint32_t GetTransferFamilyIndex() const noexcept {
                return m_transferFamilyIndex;
            }

            VulkanSwapChainSupportInfo& GetSwapChainSupportInfo() noexcept{
                return m_swapChainInfo;
            }

            inline void SetDepthBufferFormat(vk::Format format) noexcept {
                m_depthBufferFormat = format;
            }

            constexpr vk::Format GetDepthBufferFormat() const noexcept {
                return m_depthBufferFormat;
            }

            void ReQuerySwapChainSupport(vk::SurfaceKHR surface) noexcept;

        private:
            struct VulkanPhysicalDeviceRequirements{
                bool Graphics{false};
                bool Present{false};
                bool Compute{false};
                bool Transfer{false};
                bool SamplerAnisotropy{false};
                bool DiscreteGPU{false};
                std::vector<const char*> DeviceExtensionName;
            };

        private:
            void initDevice() noexcept;
            bool selectQueueFamily(VulkanPhysicalDeviceRequirements& requirements, vk::PhysicalDevice physicalDev) noexcept;
            void selectPhysicalDevice() noexcept;
            void createLogicalDevice() noexcept;
            void createCommandPool() noexcept;

        public:
            vk::CommandPool GraphicCommandPool;

        private:
            vk::PhysicalDevice m_physicalDevice;
            vk::Device m_logicalDevice{nullptr};
            uint32_t m_graphicsFamilyIndex{}, m_computeFamilyIndex{}, m_presentFamilyIndex{}, m_transferFamilyIndex{};
            vk::Queue m_graphicsQueue{nullptr}, m_presentQueue{nullptr}, m_transferQueue{nullptr};
            
            vk::Format m_depthBufferFormat;
            VulkanSwapChainSupportInfo m_swapChainInfo;
    };

}