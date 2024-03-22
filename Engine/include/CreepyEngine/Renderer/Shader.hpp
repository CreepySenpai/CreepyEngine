#pragma once

#include <string>
#include <span>
#include <filesystem>

#include <CreepyEngine/Core/Core.hpp>
#include <glm/glm.hpp>

namespace Creepy
{
    class Shader
    {

        public:
            virtual ~Shader() noexcept = default;
            virtual void Bind() const noexcept = 0;
            virtual void UnBind() const noexcept = 0;
            virtual const std::string& GetName() const noexcept = 0;
            virtual void SetName(const std::string& name) noexcept = 0;

            virtual void SetUniformInt1(const std::string& name, int value) noexcept = 0;
            virtual void SetUniformIntArray(const std::string& name, std::span<int> values) noexcept = 0;
            virtual void SetUniformFloat1(const std::string& name, float value) noexcept = 0;
            virtual void SetUniformFloat2(const std::string& name, const glm::vec2& vec) noexcept = 0;
            virtual void SetUniformFloat3(const std::string& name, const glm::vec3& vec) noexcept = 0;
            virtual void SetUniformFloat4(const std::string& name, const glm::vec4& vec) noexcept = 0;

            virtual void SetUniformMat3(const std::string& name, const glm::mat3& matrix) noexcept = 0;
            virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) noexcept = 0;

            [[nodiscard]] static Ref<Shader> Create(const std::string& name, const std::string& vertexShaderSources, const std::string& fragmentShaderSources) noexcept;

            [[nodiscard]] static Ref<Shader> Create(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept;
    }; 

    class ShaderLibrary {
        public:
            void Add(Ref<Shader>& shader) noexcept;
            void Add(const std::string& name, Ref<Shader>& shader) noexcept;

            [[nodiscard]] Ref<Shader> Load(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept;
            [[nodiscard]] Ref<Shader> Load(const std::string& name, const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept;

            [[nodiscard]] Ref<Shader> Get(const std::string& name) const noexcept;

        private:
            std::unordered_map<std::string, Ref<Shader>> m_shaders;
    };
}