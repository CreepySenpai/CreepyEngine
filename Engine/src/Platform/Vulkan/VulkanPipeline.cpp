#include <Platform/Vulkan/VulkanPipeline.hpp>
#include <Platform/Vulkan/VulkanTypes.hpp>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>

namespace Creepy {

    VulkanPipeline::VulkanPipeline(const VulkanPipelineSpec& pipelineSpec) noexcept {

        // ViewPort State
        const auto&& viewPortState = createViewportState(pipelineSpec.Viewport, pipelineSpec.Scissor);

        // Rasterizer State
        const auto&& rasterizerState = createRasterizationState(pipelineSpec.IsWireFrame);

        // MultiSampling State
        const auto&& multiSamplesState = createMultiSamplesState();

        // Depth, Stencil Testing State
        const auto&& depthStencilState = createDepthStencilState();

        // Color Blending State
        vk::PipelineColorBlendAttachmentState colorBlendAttachmentState{};
        colorBlendAttachmentState.blendEnable = vk::True;
        colorBlendAttachmentState.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
        colorBlendAttachmentState.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
        colorBlendAttachmentState.colorBlendOp = vk::BlendOp::eAdd;
        colorBlendAttachmentState.srcAlphaBlendFactor = vk::BlendFactor::eSrcAlpha;
        colorBlendAttachmentState.dstAlphaBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
        colorBlendAttachmentState.alphaBlendOp = vk::BlendOp::eAdd;
        colorBlendAttachmentState.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

        vk::PipelineColorBlendStateCreateInfo colorBlendState{};
        colorBlendState.flags = vk::PipelineColorBlendStateCreateFlags{};
        colorBlendState.logicOpEnable = vk::False;
        colorBlendState.logicOp = vk::LogicOp::eCopy;
        colorBlendState.attachmentCount = 1;
        colorBlendState.pAttachments = &colorBlendAttachmentState;

        // Dynamic State
        constexpr const std::array dynamicStates{
            vk::DynamicState::eViewport, vk::DynamicState::eScissor,
            vk::DynamicState::eLineWidth
        };

        vk::PipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.flags = vk::PipelineDynamicStateCreateFlags{};
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        // Vertex Input State
        vk::VertexInputBindingDescription bindingDesc{};
        bindingDesc.binding = 0;
        // TODO: Change
        bindingDesc.stride = 12;
        bindingDesc.inputRate = vk::VertexInputRate::eVertex;

        vk::PipelineVertexInputStateCreateInfo vertexInputState{};
        vertexInputState.flags = vk::PipelineVertexInputStateCreateFlags{};
        vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(pipelineSpec.Attributes.size());
        vertexInputState.pVertexAttributeDescriptions = pipelineSpec.Attributes.data();
        vertexInputState.vertexBindingDescriptionCount = 1;
        vertexInputState.pVertexBindingDescriptions = &bindingDesc;

        // Input Assembly State
        const auto&& inputAssemblyState = createInputAssemblyState();

        vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.flags = vk::PipelineLayoutCreateFlags{};
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(pipelineSpec.DescriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = pipelineSpec.DescriptorSetLayouts.data();

        VULKAN_CHECK_ERROR(m_layout = VulkanDevice::GetLogicalDevice().createPipelineLayout(pipelineLayoutInfo));


        vk::GraphicsPipelineCreateInfo graphicPipelineInfo{};
        graphicPipelineInfo.flags = vk::PipelineCreateFlags{};
        graphicPipelineInfo.layout = m_layout;
        graphicPipelineInfo.stageCount = static_cast<uint32_t>(pipelineSpec.ShaderStages.size());
        graphicPipelineInfo.pStages = pipelineSpec.ShaderStages.data();
        graphicPipelineInfo.pVertexInputState = &vertexInputState;
        graphicPipelineInfo.pInputAssemblyState = &inputAssemblyState;
        graphicPipelineInfo.pViewportState = &viewPortState;
        graphicPipelineInfo.pRasterizationState = &rasterizerState;
        graphicPipelineInfo.pMultisampleState = &multiSamplesState;
        graphicPipelineInfo.pDepthStencilState = &depthStencilState;
        graphicPipelineInfo.pColorBlendState = &colorBlendState;
        graphicPipelineInfo.pDynamicState = &dynamicState;
        graphicPipelineInfo.pTessellationState = nullptr;
        graphicPipelineInfo.layout = m_layout;
        graphicPipelineInfo.renderPass = pipelineSpec.RenderPass;
        graphicPipelineInfo.subpass = 0;
        graphicPipelineInfo.basePipelineHandle = nullptr;
        graphicPipelineInfo.basePipelineIndex = -1;

        
        VULKAN_CHECK_ERROR(m_handle = VulkanDevice::GetLogicalDevice().createGraphicsPipeline(nullptr, graphicPipelineInfo).value);
        
    }

    void VulkanPipeline::Destroy(const vk::Device logicalDev) noexcept
    {
        logicalDev.destroyPipelineLayout(m_layout);
        m_layout = nullptr;
        logicalDev.destroyPipeline(m_handle);
        m_handle = nullptr;
    }

    void VulkanPipeline::Bind(const vk::CommandBuffer commandBuffer, vk::PipelineBindPoint bindingPoint) noexcept
    {
        commandBuffer.bindPipeline(bindingPoint, m_handle);
    }

    vk::PipelineViewportStateCreateInfo VulkanPipeline::createViewportState(const vk::Viewport& viewport, const vk::Rect2D& scissor) noexcept {

        vk::PipelineViewportStateCreateInfo viewPortState{};
        viewPortState.flags = vk::PipelineViewportStateCreateFlags{};
        viewPortState.viewportCount = 1;
        viewPortState.scissorCount = 1;
        viewPortState.pViewports = &viewport;
        viewPortState.pScissors = &scissor;

        return viewPortState;
    }

    vk::PipelineRasterizationStateCreateInfo VulkanPipeline::createRasterizationState(bool isWireFrame) noexcept {

        vk::PipelineRasterizationStateCreateInfo rasterizerState{};
        rasterizerState.flags = vk::PipelineRasterizationStateCreateFlags{};
        rasterizerState.rasterizerDiscardEnable = vk::False;
        rasterizerState.polygonMode = isWireFrame ? vk::PolygonMode::eLine : vk::PolygonMode::eFill;
        rasterizerState.lineWidth = 1.0f;
        rasterizerState.cullMode = vk::CullModeFlagBits::eBack;
        rasterizerState.frontFace = vk::FrontFace::eCounterClockwise;
        rasterizerState.depthBiasEnable = vk::False;
        rasterizerState.depthBiasConstantFactor = 0.0f;
        rasterizerState.depthBiasClamp = 0.0f;
        rasterizerState.depthBiasSlopeFactor = 0.0f;

        return rasterizerState;
    }

    vk::PipelineMultisampleStateCreateInfo VulkanPipeline::createMultiSamplesState() noexcept {

        vk::PipelineMultisampleStateCreateInfo multiSamplesState{};
        multiSamplesState.flags = vk::PipelineMultisampleStateCreateFlags{};
        multiSamplesState.sampleShadingEnable = vk::False;
        multiSamplesState.rasterizationSamples = vk::SampleCountFlagBits::e1;
        multiSamplesState.minSampleShading = 1.0f;
        multiSamplesState.pSampleMask = nullptr;
        multiSamplesState.alphaToCoverageEnable = vk::False;
        multiSamplesState.alphaToOneEnable = vk::False;

        return multiSamplesState;
    }

    vk::PipelineDepthStencilStateCreateInfo VulkanPipeline::createDepthStencilState() noexcept {
        vk::PipelineDepthStencilStateCreateInfo depthStencilState{};
        depthStencilState.flags = vk::PipelineDepthStencilStateCreateFlags{};
        depthStencilState.depthTestEnable = vk::True;
        depthStencilState.depthWriteEnable = vk::True;
        depthStencilState.depthCompareOp = vk::CompareOp::eLess;
        depthStencilState.depthBoundsTestEnable = vk::False;
        depthStencilState.stencilTestEnable = vk::False;

        return depthStencilState;
    }

    vk::PipelineColorBlendStateCreateInfo VulkanPipeline::createColorBlendState() noexcept {
        
        vk::PipelineColorBlendAttachmentState colorBlendAttachmentState{};
        colorBlendAttachmentState.blendEnable = vk::True;
        colorBlendAttachmentState.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
        colorBlendAttachmentState.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
        colorBlendAttachmentState.colorBlendOp = vk::BlendOp::eAdd;
        colorBlendAttachmentState.srcAlphaBlendFactor = vk::BlendFactor::eSrcAlpha;
        colorBlendAttachmentState.dstAlphaBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
        colorBlendAttachmentState.alphaBlendOp = vk::BlendOp::eAdd;
        colorBlendAttachmentState.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

        vk::PipelineColorBlendStateCreateInfo colorBlendState{};
        colorBlendState.flags = vk::PipelineColorBlendStateCreateFlags{};
        colorBlendState.logicOpEnable = vk::False;
        colorBlendState.logicOp = vk::LogicOp::eCopy;
        colorBlendState.attachmentCount = 1;
        colorBlendState.pAttachments = &colorBlendAttachmentState;

        return colorBlendState;
    }

    vk::PipelineDynamicStateCreateInfo VulkanPipeline::createDynamicState() noexcept {
        const std::array dynamicStates{
            vk::DynamicState::eViewport, vk::DynamicState::eScissor,
        };

        vk::PipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.flags = vk::PipelineDynamicStateCreateFlags{};
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        return dynamicState;
    }

    vk::PipelineVertexInputStateCreateInfo VulkanPipeline::createVertexInputState(std::span<const vk::VertexInputAttributeDescription> attributes) noexcept {
        vk::VertexInputBindingDescription bindingDesc{};
        bindingDesc.binding = 0;
        // TODO: Change
        bindingDesc.stride = 12;
        bindingDesc.inputRate = vk::VertexInputRate::eVertex;

        vk::PipelineVertexInputStateCreateInfo vertexInputState{};
        vertexInputState.flags = vk::PipelineVertexInputStateCreateFlags{};
        vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());
        vertexInputState.pVertexAttributeDescriptions = attributes.data();
        vertexInputState.vertexBindingDescriptionCount = 1;
        vertexInputState.pVertexBindingDescriptions = &bindingDesc;

        return vertexInputState;
    }

    vk::PipelineInputAssemblyStateCreateInfo VulkanPipeline::createInputAssemblyState() noexcept {
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState{};
        inputAssemblyState.flags = vk::PipelineInputAssemblyStateCreateFlags{};
        inputAssemblyState.topology = vk::PrimitiveTopology::eTriangleList;
        inputAssemblyState.primitiveRestartEnable = vk::False;

        return inputAssemblyState;
    }
}