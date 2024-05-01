#pragma once

#include <filesystem>
#include <vulkan/vulkan.hpp>

namespace Creepy {

    class VulkanShader
    {
        public:
            VulkanShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept;

        private:
            std::vector<char> readFile(const std::filesystem::path& filePath) noexcept;
            void createVertexShader() noexcept;
            void createFragmentShader() noexcept;
        private:
           vk::ShaderModule m_vertexShaderHandle{nullptr};
           vk::ShaderModule m_fragmentShaderHandle{nullptr};
           std::filesystem::path m_vertexFilePath;
           std::filesystem::path m_fragmentFilePath;
    };
}