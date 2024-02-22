// #include <fstream>
// #include <sstream>
#include <CreepyEngine/Renderer/Shader.hpp>
#include <CreepyEngine/Core.hpp>
#include <glad/glad.h>

namespace Creepy {

    Shader::Shader(const std::string& vertexShaderSources, const std::string& fragmentShaderSources) noexcept {

        m_rendererID = glCreateProgram();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        const char* vertexShaderCode = vertexShaderSources.c_str();
        glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);

        glCompileShader(vertexShader);

        GLint compiled{0};
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
        if(compiled == GL_FALSE) {
            GLint logLength{0};
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<char> logString(logLength);

            glGetShaderInfoLog(vertexShader, logLength, &logLength, &logString[0]);
            
            ENGINE_LOG_ERROR("Vertex Shader Compile Error: {}", logString.data());

            glDeleteShader(vertexShader);
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        
        const char* fragmentShaderCode = fragmentShaderSources.c_str();
        glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);

        glCompileShader(fragmentShader);

        compiled = 0;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);

        if(compiled == GL_FALSE) {
            GLint logLength{0};
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<char> logString(logLength);

            glGetShaderInfoLog(fragmentShader, logLength, &logLength, &logString[0]);
            
            ENGINE_LOG_ERROR("Fragment Shader Compile Error: {}", logString.data());

            glDeleteShader(fragmentShader);
        }

        

        glAttachShader(m_rendererID, vertexShader);
        glAttachShader(m_rendererID, fragmentShader);

        glLinkProgram(m_rendererID);

        GLint isLinked{0};

        glGetProgramiv(m_rendererID, GL_LINK_STATUS, &isLinked);

        if(isLinked == GL_FALSE){
            GLint logLength{0};
            glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<char> logString(logLength);

            glGetProgramInfoLog(m_rendererID, logLength, &logLength, &logString[0]);
            
            ENGINE_LOG_ERROR("Program Link Error: {}", logString.data());

            glDeleteShader(vertexShader);

            glDeleteShader(fragmentShader);

            glDeleteProgram(m_rendererID);
        }

        glDetachShader(m_rendererID, vertexShader);
        glDetachShader(m_rendererID, fragmentShader);

        // glDeleteShader(vertexShader);
        // glDeleteShader(fragmentShader);
    }

    Shader::~Shader() noexcept {
        glDeleteProgram(m_rendererID);
    }

    void Shader::Bind() noexcept {
        glUseProgram(m_rendererID);
    }

    void Shader::UnBind() noexcept {
        glUseProgram(0);
    }

    void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) noexcept {

        auto loc = glGetUniformLocation(m_rendererID, name.c_str());
        if(loc < 0){
            ENGINE_LOG_ERROR("Uniform {} doesn't exit!", name);
        }
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));

    }
}