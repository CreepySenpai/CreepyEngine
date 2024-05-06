#pragma once

#include <filesystem>
#include <span>
#include <vulkan/vulkan.hpp>
#include <CreepyEngine/Renderer/Shader.hpp>

namespace Creepy {

    class VulkanShader : public Shader
    {
        public:
            VulkanShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept;
            
            void Destroy() noexcept;

            constexpr std::span<const vk::PipelineShaderStageCreateInfo> GetShaderStages() const noexcept {
                return m_shaderStages;
            }

            void Bind() const noexcept {}

            void UnBind() const noexcept {}

            const std::string& GetName() const noexcept {
                return {};
            }

            void SetName(const std::string& name) noexcept {}
            
        private:
            std::vector<char> readFile(const std::filesystem::path& filePath) noexcept;
            void createVertexShader() noexcept;
            void createFragmentShader() noexcept;
        private:
           vk::ShaderModule m_vertexShaderHandle{nullptr};
           vk::ShaderModule m_fragmentShaderHandle{nullptr};
           std::vector<vk::PipelineShaderStageCreateInfo> m_shaderStages;
           std::filesystem::path m_vertexFilePath;
           std::filesystem::path m_fragmentFilePath;
    };
}