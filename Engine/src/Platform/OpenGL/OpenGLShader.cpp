#include <fstream>

#include <Platform/OpenGL/OpenGLShader.hpp>
#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>
#include <glad/glad.h>


namespace Creepy {

    OpenGLShader::OpenGLShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept : m_name{"Default"}, m_vertexFilePath{vertexPath}, m_fragmentFilePath{fragmentPath} {

        createProgram();

        ENGINE_LOG_WARNING("Create A Shader: {}", m_rendererID);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexShaderSources, const std::string& fragmentShaderSources) noexcept : m_name{name} {

        ENGINE_LOG_WARNING("Gonna Create Program");
        
        this->createProgram();

        // Compile(shaderSources);
        ENGINE_LOG_WARNING("Create A Shader: {}", m_rendererID);
    }

    OpenGLShader::~OpenGLShader() noexcept {
        ENGINE_LOG_WARNING("Delete A Shader: {}", m_rendererID);
        glDeleteProgram(m_rendererID);
    }

    void OpenGLShader::Bind() const noexcept {
        glUseProgram(m_rendererID);
    }

    void OpenGLShader::UnBind() const noexcept {
        glUseProgram(0);
    }

    std::vector<char> OpenGLShader::readFile(const std::filesystem::path& filePath) noexcept {
        std::ifstream file{filePath, std::ios::ate | std::ios::binary};

        auto fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> data;
        data.resize(fileSize);

        file.seekg(0);
        file.read(&data[0], fileSize);
        file.close();

        return data;
    }

    void OpenGLShader::createProgram() noexcept {

        ENGINE_LOG_WARNING("Gona Create Program");
        GLuint program = glCreateProgram();

        auto&& vertexData = readFile(m_vertexFilePath);
        auto&& fragmentData = readFile(m_fragmentFilePath);

        auto&& vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderBinary(1, &vertexShader, GL_SHADER_BINARY_FORMAT_SPIR_V, vertexData.data(), vertexData.size() * sizeof(char));
        glSpecializeShader(vertexShader, "main", 0, nullptr, nullptr);
        glAttachShader(program, vertexShader);

        auto&& fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderBinary(1, &fragmentShader, GL_SHADER_BINARY_FORMAT_SPIR_V, fragmentData.data(), fragmentData.size() * sizeof(char));
        glSpecializeShader(fragmentShader, "main", 0, nullptr, nullptr);
        glAttachShader(program, fragmentShader);

        ENGINE_LOG_WARNING("Start Link");

        glLinkProgram(program);

        GLint isLinked{0};

        ENGINE_LOG_WARNING("Check Error");

        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

        if(isLinked == GL_FALSE){
            GLint logLength{0};
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<char> logString(logLength);

            glGetProgramInfoLog(program, logLength, &logLength, &logString[0]);
            
            ENGINE_LOG_ERROR("Program Link Error: {}", logString.data());

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            glDeleteProgram(program);
        }

        ENGINE_LOG_WARNING("Detach Shader");

        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);

        ENGINE_LOG_WARNING("Sign To Program");

        m_rendererID = program;

        ENGINE_LOG_WARNING("Created Program");
    }
}