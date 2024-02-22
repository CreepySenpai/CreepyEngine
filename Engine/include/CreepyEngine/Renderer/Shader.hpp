#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Creepy
{
    class Shader
    {

        public:
            Shader(const std::string& vertexShaderSources, const std::string& fragmentShaderSources) noexcept;
            ~Shader() noexcept;
            void Bind() noexcept;
            void UnBind() noexcept;

            void SetUniformMat4(const std::string& name, const glm::mat4& matrix) noexcept;

        private:
            unsigned int m_rendererID;
    }; 
}