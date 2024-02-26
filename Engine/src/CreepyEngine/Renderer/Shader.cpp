#include <CreepyEngine/Renderer/Shader.hpp>
#include <Platform/OpenGL/OpenGLShader.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>

namespace Creepy {
    
    Shader* Shader::Create(const std::string &vertexShaderSources, const std::string &fragmentShaderSources) noexcept
    {
        switch (Renderer::GetRenderAPI())
        {
            case RendererAPI::API::NONE:
            {
                ENGINE_LOG_ERROR("Not support");
                return nullptr;
            }
            case RendererAPI::API::OPENGL:
            {
                return new OpenGLShader(vertexShaderSources, fragmentShaderSources);
            }
            case RendererAPI::API::VULKAN:
            {
                ENGINE_LOG_ERROR("Not support");
                return nullptr;
            }
            case RendererAPI::API::DIRECTX:
            {
                ENGINE_LOG_ERROR("Not support");
                return nullptr;
            }
        }

        std::unreachable();
        ENGINE_LOG_ERROR("Unknow API");
        return nullptr;
    }
    
    Shader* Create(const std::string& filePath) noexcept {
        switch (Renderer::GetRenderAPI())
        {
            case RendererAPI::API::NONE:
            {
                ENGINE_LOG_ERROR("Not support");
                return nullptr;
            }
            case RendererAPI::API::OPENGL:
            {
                return new OpenGLShader(filePath);
            }
            case RendererAPI::API::VULKAN:
            {
                ENGINE_LOG_ERROR("Not support");
                return nullptr;
            }
            case RendererAPI::API::DIRECTX:
            {
                ENGINE_LOG_ERROR("Not support");
                return nullptr;
            }
        }

        std::unreachable();
        ENGINE_LOG_ERROR("Unknow API");
        return nullptr;
    }
}