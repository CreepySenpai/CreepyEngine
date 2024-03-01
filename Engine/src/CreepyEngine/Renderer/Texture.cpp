#include <CreepyEngine/Renderer/Texture.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>
#include <Platform/OpenGL/OpenGLTexture.hpp>

namespace Creepy {

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) noexcept {
        switch (Renderer::GetRenderAPI())
        {
            case RendererAPI::API::NONE: {
                ENGINE_LOG_ERROR("");
            }
            case RendererAPI::API::OPENGL: {
                return std::make_shared<OpenGLTexture2D>(width, height);
            }
            case RendererAPI::API::VULKAN: {
                ENGINE_LOG_ERROR("");
            }
            case RendererAPI::API::DIRECTX: {
                ENGINE_LOG_ERROR("");
            }
        }

            std::unreachable();
            ENGINE_LOG_ERROR("");
            
            return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& filePath) noexcept {
        switch (Renderer::GetRenderAPI())
        {
            case RendererAPI::API::NONE: {
                ENGINE_LOG_ERROR("");
            }
            case RendererAPI::API::OPENGL: {
                return std::make_shared<OpenGLTexture2D>(filePath);
            }
            case RendererAPI::API::VULKAN: {
                ENGINE_LOG_ERROR("");
            }
            case RendererAPI::API::DIRECTX: {
                ENGINE_LOG_ERROR("");
            }
        }

        std::unreachable();
        ENGINE_LOG_ERROR("");
        
        return nullptr;
    }

}