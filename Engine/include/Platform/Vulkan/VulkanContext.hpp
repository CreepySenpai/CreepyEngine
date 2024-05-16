#pragma once

#include <memory>
#include <CreepyEngine/Renderer/GraphicContext.hpp>
#include <vulkan/vulkan.hpp>
#include "VulkanCommandBuffer.hpp"
#include "VulkanFence.hpp"

class GLFWwindow;

namespace Creepy {

    class TimeStep;
    class VulkanDevice;
    class VulkanSwapChain;

    // TODO: Remove
    class VulkanShader;
    class VulkanImage;
    class VulkanPipeline;
    class VulkanDescriptorPool;
    class VulkanDescriptorSetLayout;
    class VulkanDescriptorSet;

    class VulkanContext : public GraphicContext
    {
        public:
            VulkanContext(GLFWwindow* windowHandle) noexcept;

            void Init() noexcept override;
            void SwapBuffers() noexcept override;
            void Update(TimeStep timeStep) noexcept override;
            void ShutDown() noexcept override;

            void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept;

            constexpr vk::Instance GetVulkanInstance() const noexcept {
                return m_instance;
            }

            constexpr vk::SurfaceKHR GetSurface() const noexcept {
                return m_surface;
            }

            std::shared_ptr<VulkanSwapChain> GetSwapChain() const noexcept;

            std::shared_ptr<VulkanDevice> GetDevices() const noexcept;

            void BeginFrame(TimeStep timeStep) noexcept;
            void EndFrame() noexcept;

            constexpr inline void SetCurrentFrame(uint32_t newFrame) noexcept {
                m_currentFrame = newFrame;
            }

        public:
            static VulkanContext* GetInstance() noexcept {
                return s_instance;
            }

            static constexpr uint32_t GetCurrentFrame() noexcept {
                return s_instance->m_currentFrame;
            }

            static constexpr uint32_t GetCurrentImageIndex() noexcept {
                return s_instance->m_currentImageIndex;
            }

        private:
            static VulkanContext* s_instance;

        private:
            void initInstance() noexcept;
            void initDebugMessage() noexcept;
            void initSurface() noexcept;
            void initDevice() noexcept;
            void initSwapChain() noexcept;
            void createCommandBuffer() noexcept;
            void createSyncObject() noexcept;
            void recreateSwapChain() noexcept;
            void createDrawImage() noexcept;

        private:
            void transitionImage(vk::Image image, vk::ImageLayout currentLayout, vk::ImageLayout newLayout) noexcept;

        private:
            // TODO: Remove
            void createDescriptor() noexcept;
            void createShader() noexcept;
            void createPipeline() noexcept;
        
        public:
            GLFWwindow* m_windowHandle{nullptr};
            vk::Instance m_instance{nullptr};
        //     vk::AllocationCallbacks Allocator{nullptr};
            vk::DispatchLoaderDynamic m_dynamicLoader;  // must not be null
            vk::DebugUtilsMessengerEXT m_debugUtils{nullptr};
            vk::SurfaceKHR m_surface{nullptr};
            std::shared_ptr<VulkanDevice> m_devices{nullptr};
            std::shared_ptr<VulkanSwapChain> m_swapChains{nullptr};
            uint32_t m_currentFrame{0};
            uint32_t m_currentImageIndex{0};
            uint32_t FrameBufferWidth{1600};
            uint32_t FrameBufferHeight{900};

            bool m_isRecreatingSwapChain{false};

            std::vector<VulkanCommandBuffer> m_graphicCommandBuffers;

            std::vector<vk::Semaphore> m_imagesAvailable;
            std::vector<vk::Semaphore> m_queuesComplete;

            std::vector<VulkanFence> m_inFlights;

        private:
            // TODO: Remove
            std::shared_ptr<VulkanShader> m_nahShader{nullptr};
            std::shared_ptr<VulkanPipeline> m_pipeLine{nullptr};
            std::shared_ptr<VulkanDescriptorPool> m_descriptorPool{nullptr};
            std::shared_ptr<VulkanDescriptorSetLayout> m_descriptorSetLayout{nullptr};
            std::shared_ptr<VulkanDescriptorSet> m_descriptorSet{nullptr};
            std::shared_ptr<VulkanImage> m_drawImage{nullptr};
            std::shared_ptr<VulkanImage> m_depthImage{nullptr};

        private:
            uint32_t m_cacheFrameBufferWidth{1600};
            uint32_t m_cacheFrameBufferHeight{900};
    };
    
}