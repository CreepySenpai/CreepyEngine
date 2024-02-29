#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Renderer/Buffer.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>
#include <Platform/OpenGL/OpenGLBuffer.hpp>

namespace Creepy
{
    Ref<VertexBuffer> VertexBuffer::Create(float* vertex, uint32_t size) {

        switch (Renderer::GetRenderAPI())
        {
            case RendererAPI::API::NONE: {
                ENGINE_LOG_ERROR("Not support none!");
                return nullptr;
            }
            case RendererAPI::API::OPENGL: {
                return std::make_shared<OpenGLVertexBuffer>(vertex, size);
            }
            case RendererAPI::API::VULKAN : {
                ENGINE_LOG_ERROR("Not support vulkan!");
                return nullptr;
            }
            case RendererAPI::API::DIRECTX : {
                ENGINE_LOG_ERROR("Not support directx!");
                return nullptr;
            }
        }

        ENGINE_LOG_ERROR("Error Wrong Render API!");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* index, uint32_t count) {

        switch (Renderer::GetRenderAPI())
        {
            case RendererAPI::API::NONE: {
                ENGINE_LOG_ERROR("Not support none!");
                return nullptr;
            }
            case RendererAPI::API::OPENGL: {
                return std::make_shared<OpenGLIndexBuffer>(index, count);
            }
            case RendererAPI::API::VULKAN : {
                ENGINE_LOG_ERROR("Not support vulkan!");
                return nullptr;
            }
            case RendererAPI::API::DIRECTX : {
                ENGINE_LOG_ERROR("Not support directx!");
                return nullptr;
            }
        }

        ENGINE_LOG_ERROR("Error Wrong Render API!");
        return nullptr;

    }

}
