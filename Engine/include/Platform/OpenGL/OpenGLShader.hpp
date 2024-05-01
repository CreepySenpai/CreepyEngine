#pragma once

#include <unordered_map>
#include <filesystem>
#include <CreepyEngine/Renderer/Shader.hpp>

namespace Creepy {

    class OpenGLShader : public Shader
    {
        public:
            OpenGLShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept;
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
            
        private:
            unsigned int m_rendererID;
            std::string m_name;
            std::filesystem::path m_vertexFilePath;
            std::filesystem::path m_fragmentFilePath;

            std::vector<char> readFile(const std::filesystem::path& filePath) noexcept;
            void createProgram() noexcept;

            
    };

}