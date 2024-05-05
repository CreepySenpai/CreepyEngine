#pragma once

#include <vector>
#include <span>
#include <initializer_list>
#include <vulkan/vulkan.hpp>

namespace Creepy {

    class VulkanDescriptorPool{
        public:
            VulkanDescriptorPool() noexcept;

            void Destroy() noexcept;

            static vk::DescriptorPool GetDescriptorPool() noexcept;

        private:
            static VulkanDescriptorPool* s_instance;

        private:
            vk::DescriptorPool m_descriptorPoolHandle;
    };


    struct VulkanDescriptorSetLayoutSpec;

    class VulkanDescriptorSetLayout
    {
        public:
            VulkanDescriptorSetLayout(const std::initializer_list<VulkanDescriptorSetLayoutSpec> layouts) noexcept;
            
            constexpr vk::DescriptorSetLayout GetLayout() const noexcept {
                return m_descriptorSetLayout;
            }

            void Destroy() noexcept;

        private:
            vk::DescriptorSetLayout m_descriptorSetLayout;
    };

    struct VulkanWriteDescriptorSetSpec;

    class VulkanDescriptorSet
    {
        public:
            VulkanDescriptorSet(std::span<const vk::DescriptorSetLayout> layouts) noexcept;

            constexpr std::span<const vk::DescriptorSet> GetDescriptorSets() const noexcept {
                return m_descriptorSets;
            }

            void Destroy() noexcept;

            void Bind(const vk::CommandBuffer commandBuffer, uint32_t index, vk::PipelineBindPoint bindPoint, vk::PipelineLayout lay, uint32_t firstSet) noexcept;
            
            void Write(const std::initializer_list<VulkanWriteDescriptorSetSpec> writeSpec) noexcept;

        private:
            std::vector<vk::DescriptorSet> m_descriptorSets;
    };
}