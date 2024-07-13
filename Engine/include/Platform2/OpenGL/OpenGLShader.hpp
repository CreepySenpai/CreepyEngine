#pragma once

#include <unordered_map>
#include <filesystem>
#include <CreepyEngine/Renderer/Shader.hpp>


namespace Creepy {
    
    class OpenGLShader2 : public Shader2{
        friend class Shader2;
        
        public:
            OpenGLShader2(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept;
            // TODO: Remove?
            OpenGLShader2(const std::string& name, const std::string& vertexShaderSources, const std::string& fragmentShaderSources) noexcept;

            ~OpenGLShader2() noexcept;
        private:
            void BindImpl() const noexcept;
            void UnBindImpl() const noexcept;

            inline const std::string& GetNameImpl() const noexcept {
                return m_name;
            }

            inline void SetNameImpl(const std::string& name) noexcept {
                m_name = name;
            }

            std::vector<char> readFile(const std::filesystem::path& filePath) noexcept;
            void createProgram() noexcept;

        private:
            unsigned int m_rendererID;
            std::string m_name;
            std::filesystem::path m_vertexFilePath;
            std::filesystem::path m_fragmentFilePath;
    };

}