#pragma once

#include <vulkan/vulkan.hpp>

namespace Creepy {

    enum class CommandBufferState{
        READY, RECORDING, IN_RENDER_PASS, RECORDING_ENDED, SUBMITTED, NOT_ALLOCATED
    };

    class VulkanCommandBuffer
    {
        public:
            constexpr VulkanCommandBuffer() noexcept = default;

            VulkanCommandBuffer(vk::Device logicalDev, vk::CommandPool commandPool, vk::CommandBufferLevel level) noexcept;
            
            void Free(vk::Device logicalDev, vk::CommandPool commandPool) noexcept;

            void Begin(bool isSingleUse, bool isRenderPassContinue, bool isSimultaneousUse) noexcept;

            void UpdateSubmitted() noexcept;

            void End() noexcept;

            void Reset() noexcept;

            constexpr vk::CommandBuffer& GetHandle() noexcept {
                return m_handle;
            }

            constexpr vk::CommandBuffer GetHandle() const noexcept {
                return m_handle;
            }

            constexpr void SetState(CommandBufferState state) noexcept {
                m_state = state;
            }

        private:
            vk::CommandBuffer m_handle{nullptr};
            CommandBufferState m_state{CommandBufferState::NOT_ALLOCATED};
    };
}