#pragma once

#include <CreepyEngine/Renderer/GraphicContext.hpp>
#include "VulkanTypes.hpp"
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

        private:
            void initInstance() noexcept;
            void initDebugMessage() noexcept;
            void initSurface() noexcept;
            void initDevice() noexcept;

        public:
            GLFWwindow* m_windowHandle;
            vk::Instance m_instance{nullptr};
            vk::AllocationCallbacks m_allocator{nullptr};
            vk::DispatchLoaderDynamic m_dynamicLoader;  // must not be null
            vk::DebugUtilsMessengerEXT m_debugUtils{nullptr};
            vk::SurfaceKHR m_surface{nullptr};
            VulkanDevice m_device{};
    };
    
}