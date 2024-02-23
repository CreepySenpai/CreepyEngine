#pragma once

#include <CreepyEngine/Renderer/Shader.hpp>
#include <glm/glm.hpp>

namespace Creepy {

    class OpenGLShader : public Shader
    {
        public:
            OpenGLShader(const std::string& vertexShaderSources, const std::string& fragmentShaderSources) noexcept;
            ~OpenGLShader() noexcept;
            void Bind() const noexcept override;
            void UnBind() const noexcept override;
            
            void SetUniformInt1(const std::string& name, int value) noexcept;
            
            void SetUniformFloat1(const std::string& name, float value) noexcept;
            void SetUniformFloat2(const std::string& name, const glm::vec2& vec) noexcept;
            void SetUniformFloat3(const std::string& name, const glm::vec3& vec) noexcept;
            void SetUniformFloat4(const std::string& name, const glm::vec4& vec) noexcept;

            void SetUniformMat3(const std::string& name, const glm::mat3& matrix) noexcept;
            void SetUniformMat4(const std::string& name, const glm::mat4& matrix) noexcept;
        private:
            unsigned int m_rendererID;
    };

}