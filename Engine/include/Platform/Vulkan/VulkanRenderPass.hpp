#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

namespace Creepy{

    enum class RenderPassState {
        READY, RECORDING, IN_RENDER_PASS, RECORDING_ENDED, SUBMITTED, NOT_ALLOCATED
    };

    struct VulkanRenderPassSpec;

    class VulkanCommandBuffer;

    class VulkanRenderPass
    {
        public:
            VulkanRenderPass(const VulkanRenderPassSpec& renderPassSpec) noexcept;

            void Destroy() noexcept;

            void Begin(VulkanCommandBuffer& commandBuffer, vk::Framebuffer frameBuffer) noexcept;

            void End(VulkanCommandBuffer& commandBuffer) noexcept;
        
            constexpr vk::RenderPass GetHandle() const noexcept {
                return m_handle;
            }

            glm::vec4& GetRenderArea() noexcept {
                return m_renderArea;
            }

        private:
            void init() noexcept;
            
        private:
            vk::RenderPass m_handle{nullptr};
            glm::vec4 m_renderArea{};
            glm::vec4 m_clearColor{};
            float m_depth{};
            uint32_t m_stencil{};
            
    };
}