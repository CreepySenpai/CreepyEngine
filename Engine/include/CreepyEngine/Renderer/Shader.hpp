#pragma once

#include <string>


namespace Creepy
{
    class Shader
    {

        public:
            Shader(const std::string& vertexShaderSources, const std::string& fragmentShaderSources) noexcept;
            ~Shader() noexcept;
            void Bind() noexcept;
            void UnBind() noexcept;
        private:
            unsigned int m_rendererID;
    }; 
}