#include <fstream>
#include <Platform/Vulkan/VulkanShader.hpp>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <CreepyEngine/Utils/VulkanUtils.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>

namespace Creepy {

    VulkanShader::VulkanShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept 
        : m_vertexFilePath{vertexPath}, m_fragmentFilePath{fragmentPath} {
        
        createVertexShader();
        createFragmentShader();
    }

    void VulkanShader::Destroy() noexcept {
        VulkanDevice::GetLogicalDevice().destroyShaderModule(m_vertexShaderHandle);
        m_vertexShaderHandle = nullptr;
        VulkanDevice::GetLogicalDevice().destroyShaderModule(m_fragmentShaderHandle);
        m_fragmentShaderHandle = nullptr;
    }

    std::vector<char> VulkanShader::readFile(const std::filesystem::path& filePath) noexcept {
        std::ifstream file{filePath, std::ios::ate | std::ios::binary};

        auto fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> data;
        data.resize(fileSize);

        file.seekg(0);
        file.read(data.data(), fileSize);
        file.close();

        return data;
    }

    void VulkanShader::createVertexShader() noexcept {
        auto&& vertexData = readFile(m_vertexFilePath);

        vk::ShaderModuleCreateInfo vertexInfo{};
        vertexInfo.flags = vk::ShaderModuleCreateFlags{};
        vertexInfo.codeSize = vertexData.size();
        vertexInfo.pCode = reinterpret_cast<uint32_t*>(vertexData.data());

        VULKAN_CHECK_ERROR(m_vertexShaderHandle = VulkanDevice::GetLogicalDevice().createShaderModule(vertexInfo));

        vk::PipelineShaderStageCreateInfo vertexStage{};
        vertexStage.flags = vk::PipelineShaderStageCreateFlags{};
        vertexStage.pName = "main";
        vertexStage.stage = vk::ShaderStageFlagBits::eVertex;
        vertexStage.module = m_vertexShaderHandle;

        m_shaderStages.emplace_back(vertexStage);
    }

    void VulkanShader::createFragmentShader() noexcept {
        auto&& fragment = readFile(m_fragmentFilePath);

        vk::ShaderModuleCreateInfo fragmentInfo{};
        fragmentInfo.flags = vk::ShaderModuleCreateFlags{};
        fragmentInfo.codeSize = fragment.size();
        fragmentInfo.pCode = reinterpret_cast<uint32_t*>(fragment.data());

        VULKAN_CHECK_ERROR(m_fragmentShaderHandle = VulkanDevice::GetLogicalDevice().createShaderModule(fragmentInfo));

        vk::PipelineShaderStageCreateInfo fragmentStage{};
        fragmentStage.flags = vk::PipelineShaderStageCreateFlags{};
        fragmentStage.pName = "main";
        fragmentStage.stage = vk::ShaderStageFlagBits::eFragment;
        fragmentStage.module = m_fragmentShaderHandle;

        m_shaderStages.emplace_back(fragmentStage);
    }

}