#pragma once

#include <vulkan/vulkan.hpp>

namespace Creepy {

    class VulkanFence
    {
        public:
            VulkanFence(bool isSignaled) noexcept;

            void Wait(uint64_t timeOut) noexcept;
            void Reset() noexcept;
            void Destroy() noexcept;

            constexpr vk::Fence GetHandle() const noexcept {
                return m_handle;
            }
        private:
            vk::Fence m_handle;
            bool m_isSignaled;
    };
}