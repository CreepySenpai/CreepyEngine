#include <Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>

namespace Creepy {
    
    void VulkanRendererAPI::Init() noexcept{
        
    }

    void VulkanRendererAPI::SetClearColor(const glm::vec4& color) noexcept {

    }

    void VulkanRendererAPI::Clear() noexcept {

    }

    void VulkanRendererAPI::EnableDepth() noexcept {

    }

    void VulkanRendererAPI::DisableDepth() noexcept {

    }

    void VulkanRendererAPI::DrawIndex(const Ref<VertexArray>& vertexArray, uint32_t indexCount) noexcept {}

    void VulkanRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) noexcept {}

    void VulkanRendererAPI::DrawInstances(const Ref<VertexArray>& vertexArray, uint32_t indexCount, uint32_t instanceCount) noexcept {}

    void VulkanRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept {
        ENGINE_LOG_WARNING("Call Set ViewPort: {} - {} - {} - {}", x, y, width, height);
        VulkanContext::GetInstance()->SetViewPort(x, y, width, height);
    }

    void VulkanRendererAPI::SetLineThickness(float thickness) noexcept {}

}