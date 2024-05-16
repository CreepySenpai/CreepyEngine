#include <Platform/Vulkan/VulkanFrameBuffer.hpp>
#include <Platform/Vulkan/VulkanTypes.hpp>
#include <Platform/Vulkan/VulkanContext.hpp>
#include <Platform/Vulkan/VulkanDevice.hpp>
#include <CreepyEngine/Debug/VulkanErrorHandle.hpp>
#include <CreepyEngine/Utils/FrameBufferUtils.hpp>

namespace Creepy {

    constexpr vk::Format ConvertTextureFormatToVulkanDataType(FrameBufferTextureFormat format) noexcept {

        switch(format) {
            case FrameBufferTextureFormat::RGBA16_SFLOAT:
                return vk::Format::eR16G16B16A16Sfloat;
            case FrameBufferTextureFormat::RED_INT:
            case FrameBufferTextureFormat::GREEN_INT:
            case FrameBufferTextureFormat::BLUE_INT:
                return vk::Format::eR32Sint;
            case FrameBufferTextureFormat::DEPTH32:
                return vk::Format::eD32Sfloat;
            case FrameBufferTextureFormat::DEPTH24STENCIL8:
                return vk::Format::eD24UnormS8Uint;
        }

        std::unreachable();
    }

    VulkanFrameBuffer::VulkanFrameBuffer(const FrameBufferSpecification& frameBufferSpec) noexcept : m_frameBufferSpecs{frameBufferSpec} {
        
        for(auto&& format : frameBufferSpec.Attachments.AttachmentList){
            if(!IsDepthFormat(format.TextureFormat)){
                m_colorTextureFormats.emplace_back(format);
            }
            else {
                m_depthTextureFormat = format;
            }
        }

        this->Invalidate();

    }

    void VulkanFrameBuffer::Invalidate() noexcept {
        
        const bool isMultiSamples = m_frameBufferSpecs.Samples > 1;

        if(!m_colorTextureFormats.empty()){

            m_colorAttachments.resize(m_colorTextureFormats.size());

            for(size_t i{}; i < m_colorTextureFormats.size(); i++){
                VulkanImageSpec colorImgSpec{};
                colorImgSpec.Width = m_frameBufferSpecs.Width;
                colorImgSpec.Height = m_frameBufferSpecs.Height;
                colorImgSpec.Aspect = vk::ImageAspectFlagBits::eColor;
                colorImgSpec.MemoryFlags = vk::MemoryPropertyFlagBits::eDeviceLocal;
                colorImgSpec.ImageType = vk::ImageType::e2D;
                colorImgSpec.IsCreateView = true;
                colorImgSpec.Tiling = vk::ImageTiling::eOptimal;
                colorImgSpec.Usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eStorage;
                colorImgSpec.Format = ConvertTextureFormatToVulkanDataType(m_colorTextureFormats.at(i).TextureFormat);
                colorImgSpec.LogicalDev = VulkanDevice::GetLogicalDevice();
                m_colorAttachments.at(i) = VulkanImage(colorImgSpec);
            }
        }

        if(m_depthTextureFormat.TextureFormat != FrameBufferTextureFormat::NONE){
            VulkanImageSpec depthImg{};
            depthImg.Width = m_frameBufferSpecs.Width;
            depthImg.Height = m_frameBufferSpecs.Height;
            depthImg.Aspect = vk::ImageAspectFlagBits::eDepth;
            depthImg.MemoryFlags = vk::MemoryPropertyFlagBits::eDeviceLocal;
            depthImg.ImageType = vk::ImageType::e2D;
            depthImg.Tiling = vk::ImageTiling::eOptimal;
            depthImg.Usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
            depthImg.LogicalDev = VulkanDevice::GetLogicalDevice();
            depthImg.IsCreateView = true;

            switch (m_depthTextureFormat.TextureFormat)
            {
                case FrameBufferTextureFormat::DEPTH24STENCIL8:
                    depthImg.Format = vk::Format::eD24UnormS8Uint;
                    break;
                case FrameBufferTextureFormat::DEPTH32:
                    depthImg.Format = vk::Format::eD32Sfloat;
                    break;
            }

            m_depthAttachment = VulkanImage{depthImg};
        }

    }

    constexpr uint32_t maxFrameSize = 9000u;

    void VulkanFrameBuffer::Resize(uint32_t width, uint32_t height) noexcept {
        if((width == 0) || (height == 0) || (width > maxFrameSize) || (height > maxFrameSize)){
            return;
        }
        
        m_frameBufferSpecs.Width = width;
        m_frameBufferSpecs.Height = height;

        this->Invalidate();
    }

    void VulkanFrameBuffer::Destroy(const vk::Device logicalDev) noexcept {
        m_depthAttachment.Destroy(logicalDev);

        for(auto& colorImg : m_colorAttachments){
            colorImg.Destroy(logicalDev);
        }
    }

    void VulkanFrameBuffer::BeginFrame() noexcept {
        // TODO: Get Command Buffer
        vk::CommandBuffer comd;

        std::vector<vk::RenderingAttachmentInfo> colorAttachmentInfos;
        colorAttachmentInfos.reserve(m_colorAttachments.size());

        constexpr vk::ClearValue* clearValue{nullptr};

        for(auto&& colorAttachment : m_colorAttachments) {
            vk::RenderingAttachmentInfo colorAttachmentInfo{};
            colorAttachmentInfo.imageView = colorAttachment.GetImageView();
            colorAttachmentInfo.imageLayout = vk::ImageLayout::eGeneral;
            
            colorAttachmentInfo.loadOp = vk::AttachmentLoadOp::eLoad;
            colorAttachmentInfo.storeOp = vk::AttachmentStoreOp::eStore;

            // TODO: Use for clear screen
            // if(clear){
            // colorAttachmentInfo.loadOp = vk::AttachmentLoadOp::eClear;
            //     info.clearValue = *clear;
            // }

            colorAttachmentInfos.emplace_back(std::move(colorAttachmentInfo));
        }


        vk::RenderingAttachmentInfo depthAttachmentInfo{};
        depthAttachmentInfo.imageView = m_depthAttachment.GetImageView();
        depthAttachmentInfo.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal;
        depthAttachmentInfo.loadOp = vk::AttachmentLoadOp::eClear;
        depthAttachmentInfo.storeOp = vk::AttachmentStoreOp::eStore;
        depthAttachmentInfo.clearValue.depthStencil.depth = 0.0f;

        vk::RenderingInfo renderingInfo{};
        renderingInfo.flags = vk::RenderingFlags{};
        renderingInfo.colorAttachmentCount = static_cast<uint32_t>(colorAttachmentInfos.size());
        renderingInfo.pColorAttachments = colorAttachmentInfos.data();
        renderingInfo.pDepthAttachment = &depthAttachmentInfo;
        renderingInfo.layerCount = 1;

        renderingInfo.renderArea.extent.width = m_frameBufferSpecs.Width;
        renderingInfo.renderArea.extent.height = m_frameBufferSpecs.Height;

        comd.beginRendering(renderingInfo);
    }

    void VulkanFrameBuffer::EndFrame() noexcept {
        // TODO: Get Command Buffer
        vk::CommandBuffer comd;

        comd.endRendering();
    }
}