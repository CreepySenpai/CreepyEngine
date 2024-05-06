#include <CreepyEngine/Renderer/Texture.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>
#include <Platform/OpenGL/OpenGLTexture.hpp>

namespace Creepy {

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) noexcept {

        if constexpr(UseOpenGLAPI){
            return std::make_shared<OpenGLTexture2D>(width, height);
        }
        else if constexpr(UseVulkanAPI){
            return nullptr;
        }
        else{
            ENGINE_LOG_ERROR("Wrong API");
            return nullptr;
        }

    }

    Ref<Texture2D> Texture2D::Create(const std::filesystem::path& filePath) noexcept {
        
        if(!std::filesystem::exists(filePath)){
            ENGINE_LOG_ERROR("Assets {} don't exits", filePath.string());
            return nullptr;
        }

        if constexpr(UseOpenGLAPI){
            return std::make_shared<OpenGLTexture2D>(filePath);
        }

        else if constexpr(UseVulkanAPI){
            return nullptr;
        }
        else{
            ENGINE_LOG_ERROR("Wrong API");
        
            return nullptr;
        }
        
    }

}