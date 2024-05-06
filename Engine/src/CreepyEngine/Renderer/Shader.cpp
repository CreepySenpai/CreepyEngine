#include <CreepyEngine/Renderer/Shader.hpp>
#include <Platform/OpenGL/OpenGLShader.hpp>
#include <Platform/Vulkan/VulkanShader.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>

namespace Creepy {
    
    Ref<Shader> Shader::Create(const std::filesystem::path& vertexPath, const std::filesystem::path&  fragmentPath) noexcept {
        
        if(!std::filesystem::exists(vertexPath) || !std::filesystem::exists(fragmentPath)){
            ENGINE_LOG_ERROR("Error Shader Path {}, {}", vertexPath.string(), fragmentPath.string());
            return nullptr;
        }

        if constexpr(UseOpenGLAPI){
            return std::make_shared<OpenGLShader>(vertexPath, fragmentPath);
        }
        else if constexpr(UseVulkanAPI){
            return std::make_shared<VulkanShader>(vertexPath, fragmentPath);
        }
        else{
            ENGINE_LOG_ERROR("Unknow API");
            return nullptr;
        } 
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

    Ref<Shader> ShaderLibrary::Load(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept {
        auto shader = Shader::Create(vertexPath, fragmentPath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string &name, const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept {
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