#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Renderer/Buffer.hpp>
#include <Platform/OpenGL/OpenGLBuffer.hpp>
#include <Platform/Vulkan/VulkanBuffer.hpp>

namespace Creepy
{
    Ref<VertexBuffer> VertexBuffer::Create(const void* data, uint32_t size) noexcept {
        if constexpr(UseOpenGLAPI){
            return std::make_shared<OpenGLVertexBuffer>(data, size);
        }

        else if constexpr(UseVulkanAPI){
            return std::make_shared<VulkanVertexBuffer>(data, size);
        }
        else{
            ENGINE_LOG_ERROR("Error Wrong Render API!");
            return nullptr;
        }
    }

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) noexcept {
        if constexpr(UseOpenGLAPI){
            return std::make_shared<OpenGLVertexBuffer>(size);
        }
        else if constexpr(UseVulkanAPI){
            return std::make_shared<VulkanVertexBuffer>(size);
        }
        else {
            ENGINE_LOG_ERROR("Error Wrong Render API!");
            return nullptr;
        }
    }

    Ref<IndexBuffer> IndexBuffer::Create(const void* indexData, uint32_t count) noexcept {

        if constexpr(UseOpenGLAPI){
            return std::make_shared<OpenGLIndexBuffer>(indexData, count);
        }
        else if constexpr(UseVulkanAPI){
            return std::make_shared<VulkanIndexBuffer>(indexData, count);
        }
        else{
            ENGINE_LOG_ERROR("Error Wrong Render API!");
            return nullptr;
        }

    }

}
