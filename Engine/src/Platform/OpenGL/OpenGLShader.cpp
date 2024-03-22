#include <fstream>

#include <Platform/OpenGL/OpenGLShader.hpp>
#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>
#include <glad/glad.h>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>


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

    static shaderc_shader_kind ConvertOpenGLShaderTypeToShaderC(GLenum type) noexcept {
        switch(type){
            case GL_VERTEX_SHADER:
                return shaderc_glsl_vertex_shader;
            case GL_FRAGMENT_SHADER:
                return shaderc_glsl_fragment_shader;
        }

        std::unreachable();
        return {};
    }

    static const char* ConvertOpenGLShaderTypeToString(GLenum type) noexcept {
        switch(type){
            case GL_VERTEX_SHADER:
                return "GL_VERTEX_SHADER";
            case GL_FRAGMENT_SHADER:
                return "GL_FRAGMENT_SHADER";
        }

        std::unreachable();
        return "";
    }

    static const char* GetOpenGLFileExtension(GLenum type) noexcept{
         switch(type){
            case GL_VERTEX_SHADER:
                return ".opengl.vert";
            case GL_FRAGMENT_SHADER:
                return ".opengl.frag";
        }

        std::unreachable();
        return "";
    }

    static const char* GetVulkanFileExtension(GLenum type) noexcept{
         switch(type){
            case GL_VERTEX_SHADER:
                return ".vulkan.vert";
            case GL_FRAGMENT_SHADER:
                return ".vulkan.frag";
        }

        std::unreachable();
        return "";
    }

    static const char* GetCacheDirectory() noexcept {
        return "./assets/shaders";
    }

    static void CreateCacheDirectoryIfNeed() noexcept {
        std::string filePath{GetCacheDirectory()};

        if(!std::filesystem::exists(filePath)){
            std::filesystem::create_directory(filePath);
        }
    }

    OpenGLShader::OpenGLShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept : m_name{"Default"}, m_vertexFilePath{vertexPath}, m_fragmentFilePath{fragmentPath} {

        CreateCacheDirectoryIfNeed();

        auto vertexSource = ReadFile(vertexPath);
        auto fragmentSource = ReadFile(fragmentPath);

        std::unordered_map<GLenum, std::string> shaderSources;
        shaderSources[GL_VERTEX_SHADER] = vertexSource;
        shaderSources[GL_FRAGMENT_SHADER] = fragmentSource;

        // Compile(shaderMap);

        this->compileShaderToVulkan(shaderSources);
        this->compileShaderToOpenGL();
        this->createProgram();

        ENGINE_LOG_WARNING("Create A Shader: {}", m_rendererID);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexShaderSources, const std::string& fragmentShaderSources) noexcept : m_name{name} {
        std::unordered_map<GLenum, std::string> shaderSources;
        shaderSources[GL_VERTEX_SHADER] = vertexShaderSources;
        shaderSources[GL_FRAGMENT_SHADER] = fragmentShaderSources;

        this->compileShaderToVulkan(shaderSources);
        this->compileShaderToOpenGL();
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

    std::string OpenGLShader::ReadFile(const std::filesystem::path& filePath) noexcept {
        std::fstream file{filePath, std::ios::in | std::ios::binary};

        if(!file.is_open()){
            ENGINE_LOG_ERROR("Cannot open file {}!", filePath.string());
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

    void OpenGLShader::compileShaderToVulkan(const std::unordered_map<GLenum, std::string>& shaderSources) noexcept {

        shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		options.SetOptimizationLevel(shaderc_optimization_level_performance);

		m_vulkanSPIRV.clear();

        std::filesystem::path cacheDirectory{GetCacheDirectory()};
        for(auto& [type, sources] : shaderSources){

            // For cache
            std::filesystem::path cachePath = cacheDirectory;

            if(type == GL_VERTEX_SHADER){
                cachePath.append(m_vertexFilePath.filename().string() + GetVulkanFileExtension(type));
                ENGINE_LOG_WARNING("{}", cachePath.string());
            } else if(type == GL_FRAGMENT_SHADER) {

                cachePath.append(m_fragmentFilePath.filename().string() + GetVulkanFileExtension(type));
                ENGINE_LOG_WARNING("{}", cachePath.string());
            } else {
                ENGINE_LOG_WARNING("Type wrong");
            }

            std::ifstream openFileCache{cachePath, std::ios::binary};

            // Shader already cache
            if(openFileCache.is_open()){
                openFileCache.seekg(0, std::ios::end);
				auto size = openFileCache.tellg();
				openFileCache.seekg(0, std::ios::beg);

				auto& data = m_vulkanSPIRV[type];
				data.resize(size / sizeof(uint32_t));
				openFileCache.read((char*)data.data(), size);

                ENGINE_LOG_WARNING("Vulkan Has Cache");
            }

            else {
                std::string fileName = type == GL_VERTEX_SHADER ? m_vertexFilePath.string() : m_fragmentFilePath.string();
                shaderc::SpvCompilationResult compileResult = compiler.CompileGlslToSpv(sources, ConvertOpenGLShaderTypeToShaderC(type), fileName.c_str(), options);

                if(compileResult.GetCompilationStatus() != shaderc_compilation_status_success){
                    ENGINE_LOG_WARNING("Compile Shader Error: {}", compileResult.GetErrorMessage());
                }

                m_vulkanSPIRV[type] = std::vector<uint32_t>{compileResult.begin(), compileResult.end()};

                std::ofstream fileOut{cachePath, std::ios::binary};

                if(fileOut.is_open()){
                    auto& dataToWrite = m_vulkanSPIRV[type];
                    fileOut.write((char*)dataToWrite.data(), dataToWrite.size() * sizeof(uint32_t));
                    fileOut.flush();
                    fileOut.close();
                }

                ENGINE_LOG_WARNING("Vulkan Create Cache");

            }
        }
    }

    void OpenGLShader::compileShaderToOpenGL() noexcept {

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
        options.SetOptimizationLevel(shaderc_optimization_level_performance);

        std::filesystem::path cacheDirectory{GetCacheDirectory()};

        m_openGLSPIRV.clear();

        // std::filesystem::path vertex = m_vertexFilePath;
        for(auto& [type, spirv] : m_vulkanSPIRV){
            
            std::filesystem::path cachePath = cacheDirectory;

            if(type == GL_VERTEX_SHADER){

                cachePath.append(m_vertexFilePath.filename().string() + GetOpenGLFileExtension(type));
                ENGINE_LOG_WARNING("{}", cachePath.string());

            } else if(type == GL_FRAGMENT_SHADER) {

                cachePath.append(m_fragmentFilePath.filename().string() + GetOpenGLFileExtension(type));
                ENGINE_LOG_WARNING("{}", cachePath.string());

                
            } else {
                ENGINE_LOG_WARNING("Type wrong");
            }

            std::ifstream openFileCache{cachePath, std::ios::binary};
            // Have cache
            if(openFileCache.is_open()){
                
                openFileCache.seekg(0, std::ios::end);
				auto size = openFileCache.tellg();
				openFileCache.seekg(0, std::ios::beg);

				auto& data = m_openGLSPIRV[type];
				data.resize(size / sizeof(uint32_t));
				openFileCache.read((char*)data.data(), size);

                ENGINE_LOG_WARNING("OpenGL Has Cache");
            }
            // No cache
            else {
                spirv_cross::CompilerGLSL glslCompiler{spirv};

                auto&& sourceCode = glslCompiler.compile();

                std::string fileName = type == GL_VERTEX_SHADER ? m_vertexFilePath.string() : m_fragmentFilePath.string();

                shaderc::SpvCompilationResult compileResult = compiler.CompileGlslToSpv(sourceCode, ConvertOpenGLShaderTypeToShaderC(type), fileName.c_str(), options);

                if(compileResult.GetCompilationStatus() != shaderc_compilation_status_success){
                    ENGINE_LOG_WARNING("Compile OpenGL shader Error: {}", compileResult.GetErrorMessage());
                }

                m_openGLSPIRV[type] = std::vector<uint32_t>{compileResult.begin(), compileResult.end()};

                std::ofstream fileOut{cachePath, std::ios::binary};

                if(fileOut.is_open()){
                    auto& dataToWrite = m_openGLSPIRV[type];
                    fileOut.write(reinterpret_cast<char*>(dataToWrite.data()), dataToWrite.size() * sizeof(uint32_t));
                    fileOut.flush();
                    fileOut.close();
                }

                ENGINE_LOG_WARNING("OpenGL Create Cache");
            }

        }
    }


    void OpenGLShader::createProgram() noexcept {
        ENGINE_LOG_WARNING("Gona Create Program");
        GLuint program = glCreateProgram();
        ENGINE_LOG_WARNING("Size: {}", m_openGLSPIRV.size());
        std::vector<GLuint> shaderIDs;
        ENGINE_LOG_WARNING("Gona Resere Mem");
        shaderIDs.reserve(m_openGLSPIRV.size());

        ENGINE_LOG_WARNING("Gona Loop");

        // TODO: Compile Vulkan Shader
        for(auto& [type, spirv] : m_openGLSPIRV){
            GLuint shaderID = shaderIDs.emplace_back(glCreateShader(type));

            // Read File From Binary Was Gen To Create Shader
            glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
            glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
            glAttachShader(program, shaderID);
        }

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

            for(auto& shaderID : shaderIDs){
                glDeleteShader(shaderID);
            }

            glDeleteProgram(program);
        }

        ENGINE_LOG_WARNING("Detach Shader");

        for(auto& shaderID : shaderIDs){
            glDetachShader(program, shaderID);
        }

        ENGINE_LOG_WARNING("Sign To Program");

        m_rendererID = program;

        ENGINE_LOG_WARNING("Created Program");
    }
}