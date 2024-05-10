#include <iostream>
#include <Platform/Vulkan/VulkanRenderPass.hpp>
#include <Platform/Vulkan/VulkanTypes.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <Platform/Vulkan/VulkanSwapChain.hpp>
#include <Platform/Vulkan/VulkanCommandBuffer.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>

namespace Creepy{

    VulkanRenderPass::VulkanRenderPass(const VulkanRenderPassSpec& renderPassSpec) noexcept
        : m_renderArea{renderPassSpec.RenderArea}, m_clearColor{renderPassSpec.ClearColor}, m_depth{renderPassSpec.Depth}, m_stencil{renderPassSpec.Stencil}
    {
        this->init(renderPassSpec.LogicalDev);
    }

    void VulkanRenderPass::Destroy(const vk::Device logicalDev) noexcept {
        logicalDev.destroyRenderPass(m_handle);
        m_handle = nullptr;
    }

    void VulkanRenderPass::Begin(VulkanCommandBuffer& commandBuffer, vk::Framebuffer frameBuffer) noexcept {

        vk::RenderPassBeginInfo beginInfo{};
        beginInfo.renderPass = m_handle;
        beginInfo.framebuffer = frameBuffer;
        
        // std::clog << "Begin Render Pass: " << m_renderArea.x << " : " << m_renderArea.y << " : " << m_renderArea.z << " : " << m_renderArea.w << '\n';
        beginInfo.renderArea.offset.x = m_renderArea.x;
        beginInfo.renderArea.offset.y = m_renderArea.y;
        beginInfo.renderArea.extent.width = m_renderArea.z;
        beginInfo.renderArea.extent.height = m_renderArea.w;

        // vk::ClearValue clearColor = {std::array<float, 4>{renderPass.ClearColor.r, renderPass.ClearColor.g, renderPass.ClearColor.b, renderPass.ClearColor.a}};
        vk::ClearValue clearColor = {std::array<float, 4>{1.0f, 0.5f, 0.25f, 1.0f}};
        vk::ClearValue clearDepth;
        clearDepth.depthStencil = vk::ClearDepthStencilValue{m_depth, m_stencil};
        std::vector<vk::ClearValue> clearValues{
            clearColor,
            clearDepth
        };

        beginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        beginInfo.pClearValues = clearValues.data();

        commandBuffer.GetHandle().beginRenderPass(beginInfo, vk::SubpassContents::eInline);
        commandBuffer.SetState(CommandBufferState::IN_RENDER_PASS);
    }

    void VulkanRenderPass::End(VulkanCommandBuffer& commandBuffer) noexcept {
        commandBuffer.GetHandle().endRenderPass();
        commandBuffer.SetState(CommandBufferState::RECORDING);
    }

    void VulkanRenderPass::init(const vk::Device logicalDev) noexcept {
        vk::SubpassDescription subPass{};
        subPass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;

        std::array<vk::AttachmentDescription, 2> attachments;
        
        // Color Attachment
        attachments[0].flags = vk::AttachmentDescriptionFlags{};
        attachments[0].format = VulkanContext::GetInstance()->GetSwapChain()->GetImageFormat().format;
        attachments[0].samples = vk::SampleCountFlagBits::e1;
        attachments[0].loadOp = vk::AttachmentLoadOp::eClear;
        attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
        attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        attachments[0].initialLayout = vk::ImageLayout::eUndefined;
        attachments[0].finalLayout = vk::ImageLayout::ePresentSrcKHR;

        vk::AttachmentReference colorAttachRef{};
        colorAttachRef.attachment = 0;
        colorAttachRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

        subPass.flags = vk::SubpassDescriptionFlags{};
        subPass.colorAttachmentCount = 1;
        subPass.pColorAttachments = &colorAttachRef;

        // Depth Attachment
        
        attachments[1].flags = vk::AttachmentDescriptionFlags{};
        attachments[1].format = VulkanContext::GetInstance()->GetDevices()->GetDepthBufferFormat();
        attachments[1].samples = vk::SampleCountFlagBits::e1;
        attachments[1].loadOp = vk::AttachmentLoadOp::eClear;
        attachments[1].storeOp = vk::AttachmentStoreOp::eStore;
        attachments[1].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        attachments[1].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        attachments[1].initialLayout = vk::ImageLayout::eUndefined;
        attachments[1].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        vk::AttachmentReference depthAttachRef{};
        depthAttachRef.attachment = 1;
        depthAttachRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        subPass.pDepthStencilAttachment = &depthAttachRef;
        
        // Input from shader
        subPass.inputAttachmentCount = 0;
        subPass.pInputAttachments = nullptr;

        subPass.pResolveAttachments = nullptr;

        subPass.preserveAttachmentCount = 0;
        subPass.pPreserveAttachments = nullptr;

        vk::SubpassDependency dependency{};
        dependency.dependencyFlags = vk::DependencyFlags{};
        dependency.srcSubpass = vk::SubpassExternal;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.srcAccessMask = vk::AccessFlagBits::eNone;
        dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

        vk::RenderPassCreateInfo renderPassInfo{};
        renderPassInfo.flags = vk::RenderPassCreateFlags{};
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subPass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;
        
        VULKAN_CHECK_ERROR(m_handle = logicalDev.createRenderPass(renderPassInfo));
    }
}