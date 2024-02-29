#pragma once

#include <CreepyEngine/Renderer/Shader.hpp>
#include <unordered_map>

namespace Creepy {

    class OpenGLShader : public Shader
    {
        public:
            OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath) noexcept;
            OpenGLShader(const std::string& name, const std::string& vertexShaderSources, const std::string& fragmentShaderSources) noexcept;
            ~OpenGLShader() noexcept;
            void Bind() const noexcept override;
            void UnBind() const noexcept override;

            inline const std::string& GetName() const noexcept override {
                return m_name;
            }

            inline void SetName(const std::string& name) noexcept override {
                m_name = name;
            }
            
            void SetUniformInt1(const std::string& name, int value) noexcept override;
            
            void SetUniformFloat1(const std::string& name, float value) noexcept override;
            void SetUniformFloat2(const std::string& name, const glm::vec2& vec) noexcept override;
            void SetUniformFloat3(const std::string& name, const glm::vec3& vec) noexcept override;
            void SetUniformFloat4(const std::string& name, const glm::vec4& vec) noexcept override;

            void SetUniformMat3(const std::string& name, const glm::mat3& matrix) noexcept override;
            void SetUniformMat4(const std::string& name, const glm::mat4& matrix) noexcept override;
        private:
            unsigned int m_rendererID;
            std::string m_name;

            using GLenum = unsigned int;
            using GLint = int;

            std::unordered_map<std::string, GLint> m_locationCache;
            

            std::string ReadFile(const std::string& filePath) noexcept;
            void Compile(const std::unordered_map<GLenum, std::string>& shaderSources) noexcept;
    };

}