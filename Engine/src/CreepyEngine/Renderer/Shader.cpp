#include <CreepyEngine/Renderer/Shader.hpp>
#include <Platform/OpenGL/OpenGLShader.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>

namespace Creepy {
    
    Ref<Shader> Shader::Create(const std::string& name, const std::string &vertexShaderSources, const std::string &fragmentShaderSources) noexcept
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
                return std::make_shared<OpenGLShader>(name, vertexShaderSources, fragmentShaderSources);
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
    
    Ref<Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath) noexcept {
        switch (Renderer::GetRenderAPI())
        {
            case RendererAPI::API::NONE:
            {
                ENGINE_LOG_ERROR("Not support");
                return nullptr;
            }
            case RendererAPI::API::OPENGL:
            {
                return std::make_shared<OpenGLShader>(vertexPath, fragmentPath);
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

    void ShaderLibrary::Add(Ref<Shader> &shader) noexcept {
        auto& shaderName = shader->GetName();
        
        if(m_shaders.contains(shaderName)){
            ENGINE_LOG_ERROR("Shader {} already exist!!!", shaderName);
        }

        m_shaders[shaderName] = shader;

    }

    void ShaderLibrary::Add(const std::string& name, Ref<Shader>& shader) noexcept {
        if(m_shaders.contains(name)){
            ENGINE_LOG_ERROR("Shader {} already exist!!!", name);
        }

        m_shaders[name] = shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& vertexPath, const std::string& fragmentPath) noexcept {
        auto shader = Shader::Create(vertexPath, fragmentPath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string &name, const std::string& vertexPath, const std::string& fragmentPath) noexcept {
        auto shader = Shader::Create(vertexPath, fragmentPath);
        shader->SetName(name);
        Add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string &name) const noexcept {
        if(!m_shaders.contains(name)){
            ENGINE_LOG_ERROR("Shader {} not exist!!!", name);
        }
        return m_shaders.at(name);
    }
}