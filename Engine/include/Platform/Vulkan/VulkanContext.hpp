#pragma once

#include <CreepyEngine/Renderer/GraphicContext.hpp>
#include "VulkanTypes.hpp"
#include "VulkanSwapchain.hpp"
#include <vulkan/vulkan.hpp>

class GLFWwindow;

namespace Creepy {

    class VulkanContext : public GraphicContext
    {
        public:
            VulkanContext(GLFWwindow* windowHandle) noexcept;

            void Init() noexcept override;
            void SwapBuffers() noexcept override;
            void ShutDown() noexcept override;

            int FindMemoryIndex(uint32_t filterType, vk::MemoryPropertyFlags memoryFlags) noexcept;

        private:
            void initInstance() noexcept;
            void initDebugMessage() noexcept;
            void initSurface() noexcept;
            void initDevice() noexcept;
            void initSwapChain() noexcept;

        public:
            GLFWwindow* m_windowHandle;
            vk::Instance Instance{nullptr};
            vk::AllocationCallbacks Allocator{nullptr};
            vk::DispatchLoaderDynamic DynamicLoader;  // must not be null
            vk::DebugUtilsMessengerEXT m_debugUtils{nullptr};
            vk::SurfaceKHR Surface{nullptr};
            VulkanDevice Devices{};
            uint32_t FrameBufferWidth{0};
            uint32_t FrameBufferHeight{0};
            VulkanSwapchain Swapchain{};
            uint32_t ImageCount{0};
            uint32_t CurrentFrame{0};
            bool RecreateSwapchain{false};
    };
    
}