#include <CreepyEngine/Renderer/VertexArray.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>
#include <Platform/OpenGL/OpenGLVertexArray.hpp>

namespace Creepy {

    VertexArray* VertexArray::Create() noexcept {

        switch (Renderer::GetRenderAPI())
        {
            case RendererAPI::API::NONE: {
                ENGINE_LOG_ERROR("Not support none!");
                return nullptr;
            }
            case RendererAPI::API::OPENGL: {
                return new OpenGLVertexArray();
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