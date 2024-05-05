#include <Platform/Vulkan/VulkanDescriptor.hpp>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <Platform/Vulkan/VulkanTypes.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>

namespace Creepy {

    VulkanDescriptorPool* VulkanDescriptorPool::s_instance = nullptr;

    VulkanDescriptorPool::VulkanDescriptorPool() noexcept {
        s_instance = this;

        vk::DescriptorPoolSize poolSize{};
        poolSize.type = vk::DescriptorType::eUniformBuffer;

        // TODO: Change to swapChain->Img->Size
        poolSize.descriptorCount = 3;

        vk::DescriptorPoolCreateInfo poolInfo{};
        poolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;

        // TODO: Change to swapChain->Img->Size
        poolInfo.maxSets = 3;

        VULKAN_CHECK_ERROR(m_descriptorPoolHandle = VulkanDevice::GetLogicalDevice().createDescriptorPool(poolInfo));
    }

    vk::DescriptorPool VulkanDescriptorPool::GetDescriptorPool() noexcept {
        return s_instance->m_descriptorPoolHandle;
    }

    void VulkanDescriptorPool::Destroy() noexcept {
        VulkanDevice::GetLogicalDevice().destroyDescriptorPool(m_descriptorPoolHandle);
    }

    VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const std::initializer_list<VulkanDescriptorSetLayoutSpec> layouts) noexcept {

        std::vector<vk::DescriptorSetLayoutBinding> layoutBindings;
        layoutBindings.reserve(layouts.size());

        for(auto&& layout : layouts){

            vk::DescriptorSetLayoutBinding layoutBinding{};
            layoutBinding.binding = layout.Binding;
            layoutBinding.descriptorCount = layout.DescriptorCount;
            layoutBinding.descriptorType = layout.DescriptorType;
            layoutBinding.stageFlags = layout.StageFlags;
            layoutBinding.pImmutableSamplers = nullptr;

            layoutBindings.emplace_back(layoutBinding);
        }

        vk::DescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.flags = vk::DescriptorSetLayoutCreateFlags{};
        layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
        layoutInfo.pBindings = layoutBindings.data();

        VULKAN_CHECK_ERROR(m_descriptorSetLayout = VulkanDevice::GetLogicalDevice().createDescriptorSetLayout(layoutInfo));
    }

    void VulkanDescriptorSetLayout::Destroy() noexcept {
        VulkanDevice::GetLogicalDevice().destroyDescriptorSetLayout(m_descriptorSetLayout);
    }


    // Descriptor Set
    VulkanDescriptorSet::VulkanDescriptorSet(std::span<const vk::DescriptorSetLayout> layouts) noexcept {
        vk::DescriptorSetAllocateInfo allocInfo{};

        allocInfo.descriptorPool = VulkanDescriptorPool::GetDescriptorPool();
        allocInfo.descriptorSetCount = static_cast<uint32_t>(layouts.size());
        allocInfo.pSetLayouts = layouts.data();

        VULKAN_CHECK_ERROR(m_descriptorSets = VulkanDevice::GetLogicalDevice().allocateDescriptorSets(allocInfo));
    }

    void VulkanDescriptorSet::Destroy() noexcept {
        VulkanDevice::GetLogicalDevice().freeDescriptorSets(VulkanDescriptorPool::GetDescriptorPool(), m_descriptorSets);
    }

    void VulkanDescriptorSet::Bind(const vk::CommandBuffer commandBuffer, uint32_t index, vk::PipelineBindPoint bindPoint, vk::PipelineLayout lay, uint32_t firstSet) noexcept {
        commandBuffer.bindDescriptorSets(bindPoint, lay, firstSet, m_descriptorSets, nullptr);

        
    }

    void VulkanDescriptorSet::Write(const std::initializer_list<VulkanWriteDescriptorSetSpec> writeSpec) noexcept {
        std::vector< vk::WriteDescriptorSet> writeDescriptors;
        writeDescriptors.reserve(writeSpec.size());

        for(auto&& spec : writeSpec){
            vk::DescriptorBufferInfo bufferInfo{};
            bufferInfo.offset = spec.Offset;
            bufferInfo.range = spec.Range;

            vk::WriteDescriptorSet write{};
            write.dstSet = m_descriptorSets.at(spec.DescriptorSetIndex);
            write.dstBinding = 0;
            write.dstArrayElement = 0;
            write.descriptorType = spec.DescriptorType;
            write.descriptorCount = spec.DescriptorCount;
            write.pBufferInfo = &bufferInfo;

            writeDescriptors.emplace_back(write);
        }

        VulkanDevice::GetLogicalDevice().updateDescriptorSets(writeDescriptors, nullptr);
    }

}