#include <CreepyEngine/Renderer/UniformBuffer.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>
#include <Platform/OpenGL/OpenGLUniformBuffer.hpp>

namespace Creepy{

    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) noexcept {
        switch (Renderer::GetRenderAPI())
        {
            case RendererAPI::API::NONE: {
                ENGINE_LOG_ERROR("Dont Support!!!");
                return nullptr;
            }
            case RendererAPI::API::OPENGL: {
                return std::make_shared<OpenGLUniformBuffer>(size, binding);
            }
            case RendererAPI::API::VULKAN: {
                ENGINE_LOG_ERROR("Dont Support!!!");
                return nullptr;
            }
            case RendererAPI::API::DIRECTX: {
                ENGINE_LOG_ERROR("Dont Support!!!");
                return nullptr;
            }
        }

            std::unreachable();
            ENGINE_LOG_ERROR("");
            
            return nullptr;
    }

}