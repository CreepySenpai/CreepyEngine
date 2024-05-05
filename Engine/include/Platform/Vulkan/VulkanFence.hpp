#pragma once

#include <vulkan/vulkan.hpp>

namespace Creepy {

    class VulkanFence
    {
        public:
            VulkanFence(const vk::Device logicalDev, bool isSignaled) noexcept;

            void Wait(const vk::Device logicalDev, uint64_t timeOut) noexcept;
            void Reset(const vk::Device logicalDev) noexcept;
            void Destroy(const vk::Device logicalDev) noexcept;

            constexpr vk::Fence GetHandle() const noexcept {
                return m_handle;
            }
        private:
            vk::Fence m_handle;
            bool m_isSignaled;
    };
}