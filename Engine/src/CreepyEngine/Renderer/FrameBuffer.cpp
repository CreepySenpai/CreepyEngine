#include <CreepyEngine/Renderer/FrameBuffer.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>
#include <Platform/OpenGL/OpenGLFrameBuffer.hpp>

namespace Creepy {

    Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& data) noexcept {

        switch (Renderer::GetRenderAPI())
        {
            case RendererAPI::API::NONE: {
                ENGINE_LOG_ERROR("Dont Support!!!");
                return nullptr;
            }
            case RendererAPI::API::OPENGL: {
                return std::make_shared<OpenGLFrameBuffer>(data);
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
            ENGINE_LOG_ERROR("Wrong API!!!");
            
            return nullptr;
    }

}