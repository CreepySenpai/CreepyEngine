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

    OpenGLShader::OpenGLShader(const std::string& filePath) noexcept {
        auto sources = ReadFile(filePath);
        auto shaderSources = PreProcess(sources);

        Compile(shaderSources);

        auto lastSlash = filePath.find_last_of("/\\");
        
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;

        auto lastDot = filePath.rfind('.');

        auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
        
        m_name = filePath.substr(lastSlash, count);

    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexShaderSources, const std::string& fragmentShaderSources) noexcept : m_name{name} {
        std::unordered_map<GLenum, std::string> shaderSources;
        shaderSources[GL_VERTEX_SHADER] = vertexShaderSources;
        shaderSources[GL_FRAGMENT_SHADER] = fragmentShaderSources;

        Compile(shaderSources);
    }

    OpenGLShader::~OpenGLShader() noexcept {
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
    

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& sources) noexcept {
        std::unordered_map<GLenum, std::string> shaderSources;

        std::string split{"#type"};
        size_t splitLen = split.size();
        size_t pos = sources.find(split, 0);

        while(pos != std::string::npos) {
            size_t eol = sources.find_first_not_of("\r\n", pos);

            size_t begin = pos + splitLen + 1;
            
            std::string type = sources.substr(begin, eol - begin);

            size_t nextLinePos = sources.find_first_not_of("\r\n", eol);

            pos = sources.find(split, nextLinePos);

            shaderSources[GetShaderType(type)] = sources.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? sources.size() - 1 : nextLinePos));
        }

        return shaderSources;
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