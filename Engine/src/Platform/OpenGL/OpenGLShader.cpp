#include <fstream>
#include <Platform/OpenGL/OpenGLShader.hpp>
#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>
#include <glad/glad.h>


namespace Creepy {

    static GLenum GetShaderType(const std::string& type) noexcept {
        GLenum shaderType{0};
        if(type == "vertex"){
            shaderType = GL_VERTEX_SHADER;
        } else {
            shaderType = GL_FRAGMENT_SHADER;
        }

        return shaderType;
    }

    OpenGLShader::OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath) noexcept : m_name{"Default"} {
        auto vertexSource = ReadFile(vertexPath);
        auto fragmentSource = ReadFile(fragmentPath);

        std::unordered_map<GLenum, std::string> shaderMap;
        shaderMap[GL_VERTEX_SHADER] = vertexSource;
        shaderMap[GL_FRAGMENT_SHADER] = fragmentSource;

        Compile(shaderMap);
        ENGINE_LOG_WARNING("Create A Shader: {}", m_rendererID);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexShaderSources, const std::string& fragmentShaderSources) noexcept : m_name{name} {
        std::unordered_map<GLenum, std::string> shaderSources;
        shaderSources[GL_VERTEX_SHADER] = vertexShaderSources;
        shaderSources[GL_FRAGMENT_SHADER] = fragmentShaderSources;

        Compile(shaderSources);
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

    void OpenGLShader::SetUniformInt1(const std::string& name, int value) noexcept {
        if(m_locationCache.contains(name)){
            glUniform1i(m_locationCache.at(name), value);
        } else {
            auto loc = glGetUniformLocation(m_rendererID, name.c_str());
            if(loc < 0){
                ENGINE_LOG_ERROR("Uniform {} doesn't exit!", name);
                return;
            }
            glUniform1i(loc, value);
            m_locationCache[name] = loc;
        }
        
    }


    void OpenGLShader::SetUniformIntArray(const std::string& name, std::span<int> values) noexcept {

        if(m_locationCache.contains(name)){
            glUniform1iv(m_locationCache.at(name), values.size(), values.data());
        } else {
            auto loc = glGetUniformLocation(m_rendererID, name.c_str());
            if(loc < 0){
                ENGINE_LOG_ERROR("Uniform {} doesn't exit!", name);
                return;
            }
            glUniform1iv(loc, values.size(), values.data());
            m_locationCache[name] = loc;
        }
        
    }

    void OpenGLShader::SetUniformFloat1(const std::string &name, float value) noexcept {
        if(m_locationCache.contains(name)){
            glUniform1f(m_locationCache.at(name), value);
        }
        else {
            auto loc = glGetUniformLocation(m_rendererID, name.c_str());
            if(loc < 0){
                ENGINE_LOG_ERROR("Uniform {} doesn't exit!", name);
                return;
            }
            glUniform1f(loc, value);
            m_locationCache[name] = loc;
        }
    }
    void OpenGLShader::SetUniformFloat2(const std::string &name, const glm::vec2 &vec) noexcept {
        if(m_locationCache.contains(name)){
            glUniform2fv(m_locationCache.at(name), 1, glm::value_ptr(vec));
        }
        else {
            auto loc = glGetUniformLocation(m_rendererID, name.c_str());
            if(loc < 0){
                ENGINE_LOG_ERROR("Uniform {} doesn't exit!", name);
                return;
            }
            glUniform2fv(loc, 1, glm::value_ptr(vec));
            m_locationCache[name] = loc;
        }
    }
    void OpenGLShader::SetUniformFloat3(const std::string &name, const glm::vec3 &vec) noexcept {

        if(m_locationCache.contains(name)){
            glUniform3fv(m_locationCache.at(name), 1, glm::value_ptr(vec));
        }
        else {
            auto loc = glGetUniformLocation(m_rendererID, name.c_str());
            if(loc < 0){
                ENGINE_LOG_ERROR("Uniform {} doesn't exit!", name);
                return;
            }
            glUniform3fv(loc, 1, glm::value_ptr(vec));
            m_locationCache[name] = loc;
        }

    }

    void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& vec) noexcept {

        if(m_locationCache.contains(name)){
            glUniform4fv(m_locationCache.at(name), 1, glm::value_ptr(vec));
        }
        else {
            auto loc = glGetUniformLocation(m_rendererID, name.c_str());
            if(loc < 0){
                ENGINE_LOG_ERROR("Uniform {} doesn't exit!", name);
                return;
            }
            glUniform4fv(loc, 1, glm::value_ptr(vec));
            m_locationCache[name] = loc;
        }

    }

    void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& matrix) noexcept {

        if(m_locationCache.contains(name)){
            glUniformMatrix3fv(m_locationCache.at(name), 1, GL_FALSE, glm::value_ptr(matrix));
        }
        else {
            auto loc = glGetUniformLocation(m_rendererID, name.c_str());
            if(loc < 0){
                ENGINE_LOG_ERROR("Uniform {} doesn't exit!", name);
                return;
            }
            glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
            m_locationCache[name] = loc;
        }

    }

    void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) noexcept {

        if(m_locationCache.contains(name)){
            glUniformMatrix4fv(m_locationCache.at(name), 1, GL_FALSE, glm::value_ptr(matrix));
        }
        else {
            auto loc = glGetUniformLocation(m_rendererID, name.c_str());
            if(loc < 0){
                ENGINE_LOG_ERROR("Uniform {} doesn't exit!", name);
                return;
            }
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
            m_locationCache[name] = loc;
        }

    }

    std::string OpenGLShader::ReadFile(const std::string& filePath) noexcept {
        std::fstream file{filePath, std::ios::in | std::ios::binary};

        if(!file.is_open()){
            ENGINE_LOG_ERROR("Cannot open file {}!", filePath);
        }
        
        std::stringstream stream;
        stream << file.rdbuf();
        std::string result{stream.str()};

        return result;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources) noexcept {
        m_rendererID = glCreateProgram();

        std::vector<GLenum> shaderIDs;
        shaderIDs.reserve(shaderSources.size());

        for(auto& [shaderType, sources] : shaderSources) {

            GLuint shader = glCreateShader(shaderType);

            const char* sourcesCstr = sources.c_str();

            glShaderSource(shader, 1, &sourcesCstr, nullptr);

            glCompileShader(shader);

            GLint compiled{0};
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

            if(compiled == GL_FALSE) {
                ENGINE_LOG_ERROR("Shader Compile Error 1!!!");
                GLint logLength{0};
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

                std::vector<char> logString(logLength);

                glGetShaderInfoLog(shader, logLength, &logLength, &logString[0]);
                
                ENGINE_LOG_ERROR("Shader Compile Error: {}", logString.data());

                glDeleteShader(shader);
                break;
            }

            glAttachShader(m_rendererID, shader);
            shaderIDs.emplace_back(shader);
        }

        glLinkProgram(m_rendererID);

        GLint isLinked{0};

        glGetProgramiv(m_rendererID, GL_LINK_STATUS, &isLinked);

        if(isLinked == GL_FALSE){
            GLint logLength{0};
            glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<char> logString(logLength);

            glGetProgramInfoLog(m_rendererID, logLength, &logLength, &logString[0]);
            
            ENGINE_LOG_ERROR("Program Link Error: {}", logString.data());

            for(auto& shaderID : shaderIDs){
                glDeleteShader(shaderID);
            }

            glDeleteProgram(m_rendererID);
        }

        for(auto& shaderID : shaderIDs){
            glDetachShader(m_rendererID, shaderID);
        }
        
    }
}