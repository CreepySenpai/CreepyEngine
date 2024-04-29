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
    class VulkanRenderPass;

    class VulkanContext : public GraphicContext
    {
        public:
            VulkanContext(GLFWwindow* windowHandle) noexcept;

            void Init() noexcept override;
            void SwapBuffers() noexcept override;
            void Update(TimeStep timeStep) noexcept override;
            void ShutDown() noexcept override;

            int FindMemoryIndex(uint32_t filterType, vk::MemoryPropertyFlags memoryFlags) noexcept;

            void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept;

            vk::Instance GetVulkanInstance() const noexcept {
                return Instance;
            }

            vk::Device GetLogicalDevice() const noexcept;

            void BeginFrame(TimeStep timeStep) noexcept;
            void EndFrame() noexcept;

        public:
            static VulkanContext* GetInstance() noexcept {
                return s_instance;
            }

        private:
            static VulkanContext* s_instance;

        private:
            void initInstance() noexcept;
            void initDebugMessage() noexcept;
            void initSurface() noexcept;
            void initDevice() noexcept;
            void initSwapChain() noexcept;
            void initRenderPass() noexcept;
            void createFrameBuffer() noexcept;
            void createCommandBuffer() noexcept;
            void createSyncObject() noexcept;
            void recreateSwapChain() noexcept;

            

        public:
            GLFWwindow* m_windowHandle;
            vk::Instance Instance{nullptr};
        //     vk::AllocationCallbacks Allocator{nullptr};
            vk::DispatchLoaderDynamic DynamicLoader;  // must not be null
            vk::DebugUtilsMessengerEXT m_debugUtils{nullptr};
            vk::SurfaceKHR Surface{nullptr};
            std::shared_ptr<VulkanDevice> Devices{nullptr};
            std::shared_ptr<VulkanSwapChain> SwapChain{nullptr};
            std::shared_ptr<VulkanRenderPass> MainRenderPass{nullptr};
            uint32_t CurrentFrame{0};
            uint32_t CurrentImageIndex{0};
            uint32_t FrameBufferWidth{1600};
            uint32_t FrameBufferHeight{900};

            bool m_isRecreatingSwapChain{false};

            std::vector<VulkanCommandBuffer> GraphicCommandBuffers;

            std::vector<vk::Semaphore> m_imagesAvailable;
            std::vector<vk::Semaphore> m_queuesComplete;

            std::vector<VulkanFence> m_inFlights;

        private:
            uint32_t m_cacheFrameBufferWidth{1600};
            uint32_t m_cacheFrameBufferHeight{900};
    };
    
}