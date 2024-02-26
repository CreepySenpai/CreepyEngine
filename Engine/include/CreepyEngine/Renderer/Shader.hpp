#pragma once

#include <string>

namespace Creepy
{
    class Shader
    {

        public:
            virtual ~Shader() noexcept = default;
            virtual void Bind() const noexcept = 0;
            virtual void UnBind() const noexcept = 0;

            static Shader* Create(const std::string& vertexShaderSources, const std::string& fragmentShaderSources) noexcept;

            static Shader* Create(const std::string& filePath) noexcept;
    }; 
}