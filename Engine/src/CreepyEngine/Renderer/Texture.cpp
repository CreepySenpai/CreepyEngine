#include <CreepyEngine/Renderer/Texture.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>
#include <Platform/OpenGL/OpenGLTexture.hpp>

namespace Creepy {

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) noexcept {
        switch (Renderer::GetRenderAPI())
        {
            case RendererAPI::API::NONE: {
                ENGINE_LOG_ERROR("Dont Support!!!");
                return nullptr;
            }
            case RendererAPI::API::OPENGL: {
                return std::make_shared<OpenGLTexture2D>(width, height);
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

    Ref<Texture2D> Texture2D::Create(const std::filesystem::path& filePath) noexcept {
        if(!std::filesystem::exists(filePath)){
            ENGINE_LOG_ERROR("Assets {} don't exits", filePath.string());
            return nullptr;
        }
        
        switch (Renderer::GetRenderAPI())
        {
            case RendererAPI::API::NONE: {
                ENGINE_LOG_ERROR("Dont Support!!!");
                return nullptr;
            }
            case RendererAPI::API::OPENGL: {
                return std::make_shared<OpenGLTexture2D>(filePath);
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