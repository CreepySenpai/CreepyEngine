#pragma once

#include <vulkan/vulkan.hpp>

namespace Creepy {

    class VulkanFence
    {
        public:
            VulkanFence(vk::Device logicalDev, bool isSignaled) noexcept;

            void Wait(vk::Device logicalDev, uint64_t timeOut) noexcept;
            void Reset(vk::Device logicalDev) noexcept;
            void Destroy(vk::Device logicalDev) noexcept;

            constexpr vk::Fence GetHandle() const noexcept {
                return m_handle;
            }
        private:
            vk::Fence m_handle;
            bool m_isSignaled;
    };
}