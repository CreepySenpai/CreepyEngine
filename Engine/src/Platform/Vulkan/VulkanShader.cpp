#include <fstream>
#include <Platform/Vulkan/VulkanShader.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>
#include <CreepyEngine/Utils/VulkanUtils.hpp>


namespace Creepy {

    VulkanShader::VulkanShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept 
        : m_vertexFilePath{vertexPath}, m_fragmentFilePath{fragmentPath} {
        
        createVertexShader();
        createFragmentShader();
    }

    std::vector<char> VulkanShader::readFile(const std::filesystem::path& filePath) noexcept {
        std::ifstream file{filePath, std::ios::ate | std::ios::binary};

        auto fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> data;
        data.resize(fileSize);

        file.seekg(0);
        file.read(&data[0], fileSize);
        file.close();

        return data;
    }

    void VulkanShader::createVertexShader() noexcept {
        auto&& vertexData = readFile(m_vertexFilePath);

        vk::ShaderModuleCreateInfo vertexInfo{};
        vertexInfo.flags = vk::ShaderModuleCreateFlags{};
        vertexInfo.codeSize = vertexData.size();
        vertexInfo.pCode = reinterpret_cast<uint32_t*>(vertexData.data());

        try{
            m_vertexShaderHandle = VulkanContext::GetInstance()->GetLogicalDevice().createShaderModule(vertexInfo);
        }
        catch(const vk::SystemError& e){
            VulkanUtils::Log("Create Vertex Shader Error: " + std::string{e.what()});
        }
    }

    void VulkanShader::createFragmentShader() noexcept {
        auto&& fragment = readFile(m_fragmentFilePath);

        vk::ShaderModuleCreateInfo fragmentInfo{};
        fragmentInfo.flags = vk::ShaderModuleCreateFlags{};
        fragmentInfo.codeSize = fragment.size();
        fragmentInfo.pCode = reinterpret_cast<uint32_t*>(fragment.data());

        try{
            m_fragmentShaderHandle = VulkanContext::GetInstance()->GetLogicalDevice().createShaderModule(fragmentInfo);
        }
        catch(const vk::SystemError& e){
            VulkanUtils::Log("Create Fragment Shader Error: " + std::string{e.what()});
        }
    }

}