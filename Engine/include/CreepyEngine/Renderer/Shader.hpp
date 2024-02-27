#pragma once

#include <string>
#include <CreepyEngine/Core.hpp>

namespace Creepy
{
    class Shader
    {

        public:
            virtual ~Shader() noexcept = default;
            virtual void Bind() const noexcept = 0;
            virtual void UnBind() const noexcept = 0;
            virtual const std::string& GetName() const noexcept = 0;

            static Ref<Shader> Create(const std::string& name, const std::string& vertexShaderSources, const std::string& fragmentShaderSources) noexcept;

            static Ref<Shader> Create(const std::string& filePath) noexcept;
    }; 

    class ShaderLibrary {
        public:
            void Add(Ref<Shader>& shader) noexcept;
            void Add(const std::string& name, Ref<Shader>& shader) noexcept;

            Ref<Shader> Load(const std::string& filePath) noexcept;
            Ref<Shader> Load(const std::string& name, const std::string& filePath) noexcept;

            Ref<Shader> Get(const std::string& name) const noexcept;

        private:
            std::unordered_map<std::string, Ref<Shader>> m_shaders;
    };
}