#include <Platform/OpenGL/OpenGLFrameBuffer.hpp>
#include <CreepyEngine/Debug/ErrorHandle.hpp>
#include <glad/glad.h>


namespace Creepy {
    
    // TODO: Move static function to frame buffer utils
    static bool IsDepthFormat(FrameBufferTextureFormat format) noexcept {

        switch(format){
            case FrameBufferTextureFormat::DEPTH24STENCIL8:
                return true;
        }

        return false;
    }

    static GLenum TextureTarget(bool isMultiSamples) noexcept {
        return isMultiSamples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    static void CreateTextures(bool isMultiSamples, uint32_t* renderIDs, size_t count) noexcept{
        glCreateTextures(TextureTarget(isMultiSamples), count, renderIDs);
    }

    static void BindTexture(bool isMultiSamples, uint32_t textureID) noexcept {
        glBindTexture(TextureTarget(isMultiSamples), textureID);
    }

    static void AttachColorTexture(uint32_t textureID, int samples, GLenum internalFormat, GLenum dataFormat, uint32_t width, uint32_t height, size_t index) noexcept {
        bool isMultiSamples = samples > 1;

        if(isMultiSamples) {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);

            // Multi Sample Texture Dont Need Set Filter
        }
        else {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, nullptr);

            glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(textureID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(isMultiSamples), textureID, 0);
    }

    static void AttachDepthTexture(uint32_t textureID, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) noexcept {
        bool isMultiSamples = samples > 1;

        if(isMultiSamples) {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);

            // Multi Sample Texture Dont Need Set Filter
        }
        else {
            glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

            glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(textureID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(isMultiSamples), textureID, 0);
    }

    static GLenum ConvertTextureFormatToOpenGLFormat(FrameBufferTextureFormat format) noexcept {
        switch(format) {
            case FrameBufferTextureFormat::RGBA8:
                return GL_RGBA8;
            case FrameBufferTextureFormat::RED_INT:
                return GL_RED_INTEGER;
            case FrameBufferTextureFormat::GREEN_INT:
                return GL_GREEN_INTEGER;
            case FrameBufferTextureFormat::BLUE_INT:
                return GL_BLUE_INTEGER;
            
        }

        std::unreachable();
        return 0;
    }

    static GLenum ConvertTextureFormatToOpenGLDataType(FrameBufferTextureFormat format) noexcept {
        switch(format) {
            case FrameBufferTextureFormat::RGBA8:
                return GL_UNSIGNED_BYTE;
            case FrameBufferTextureFormat::RED_INT:
                [[fallthrough]];
            case FrameBufferTextureFormat::GREEN_INT:
                [[fallthrough]];
            case FrameBufferTextureFormat::BLUE_INT:
                return GL_INT;
        }

        std::unreachable();
        return 0;
    }

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& data) noexcept : m_data{data} {

        for(auto format : m_data.Attachments.AttachmentList){
            if(!IsDepthFormat(format.TextureFormat)) {
                m_colorBufferAttachmentsSpec.emplace_back(format);
            }
            else {
                m_deptBufferAttachmentSpec = format;
            }
        }
        
        this->Invalidate();

    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer() noexcept {

        glDeleteFramebuffers(1, &m_rendererID);
        glDeleteTextures(m_colorBufferAttachments.size(), m_colorBufferAttachments.data());
        glDeleteTextures(1, &m_depthBufferAttachment);
        
        ENGINE_LOG_WARNING("Delete A FrameBuffer: {}", m_rendererID);
    }

    void OpenGLFrameBuffer::Bind() noexcept {
        glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
        glViewport(0, 0, m_data.Width, m_data.Height);
    }

    void OpenGLFrameBuffer::UnBind() noexcept {
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

    static constexpr uint32_t maxFrameSize = 9000;

    void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height) noexcept {
        if((width == 0) || (height == 0) || (width > maxFrameSize) || (height > maxFrameSize)){
            return;
        }
        m_data.Width = width;
        m_data.Height = height;

        this->Invalidate();

    }

    void OpenGLFrameBuffer::Invalidate() noexcept {
        
        // Check If Frame Buffer Already Create
        if(m_rendererID){
            glDeleteFramebuffers(1, &m_rendererID);
            glDeleteTextures(m_colorBufferAttachments.size(), m_colorBufferAttachments.data());
            glDeleteTextures(1, &m_depthBufferAttachment);

            m_colorBufferAttachments.clear();
            m_depthBufferAttachment = 0;
        }

        glCreateFramebuffers(1, &m_rendererID);

        ENGINE_LOG_WARNING("Create A FrameBuffer: {}", m_rendererID);

        glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

        bool isMultiSamples = m_data.Samples > 1;
        
        if(!m_colorBufferAttachmentsSpec.empty()){

            m_colorBufferAttachments.resize(m_colorBufferAttachmentsSpec.size());

            CreateTextures(isMultiSamples, m_colorBufferAttachments.data(), m_colorBufferAttachments.size());

            // AttachMents
            for(size_t i{}; i < m_colorBufferAttachments.size(); i++){
                BindTexture(isMultiSamples, m_colorBufferAttachments[i]);

                switch(m_colorBufferAttachmentsSpec[i].TextureFormat){
                    case FrameBufferTextureFormat::RGBA8:
                        AttachColorTexture(m_colorBufferAttachments[i], m_data.Samples, GL_RGBA8, GL_RGBA, m_data.Width, m_data.Height, i);
                        break;

                    case FrameBufferTextureFormat::RED_INT:
                        AttachColorTexture(m_colorBufferAttachments[i], m_data.Samples, GL_R32I, GL_RED_INTEGER, m_data.Width, m_data.Height, i);
                        break;
                    case FrameBufferTextureFormat::GREEN_INT:
                        AttachColorTexture(m_colorBufferAttachments[i], m_data.Samples, GL_R32I, GL_GREEN_INTEGER, m_data.Width, m_data.Height, i);
                        break;
                    case FrameBufferTextureFormat::BLUE_INT:
                        AttachColorTexture(m_colorBufferAttachments[i], m_data.Samples, GL_R32I, GL_BLUE_INTEGER, m_data.Width, m_data.Height, i);
                        break;
                }

            }

        }

        if(m_deptBufferAttachmentSpec.TextureFormat != FrameBufferTextureFormat::NONE){
            CreateTextures(isMultiSamples, &m_depthBufferAttachment, 1);
            BindTexture(isMultiSamples, m_depthBufferAttachment);
            
            switch(m_deptBufferAttachmentSpec.TextureFormat){
                case FrameBufferTextureFormat::DEPTH24STENCIL8:
                    AttachDepthTexture(m_depthBufferAttachment, m_data.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_ATTACHMENT, m_data.Width, m_data.Height);
                    break;
            }
        }

        if(m_colorBufferAttachments.size() > 1){

            ENGINE_ASSERT((m_colorBufferAttachments.size() < 32), "Limit Color Buffer Is 32!!!");

            GLenum buffer[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
                               GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7,
                               GL_COLOR_ATTACHMENT8, GL_COLOR_ATTACHMENT9, GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11,
                               GL_COLOR_ATTACHMENT12, GL_COLOR_ATTACHMENT13, GL_COLOR_ATTACHMENT14,GL_COLOR_ATTACHMENT15, 
                               GL_COLOR_ATTACHMENT16, GL_COLOR_ATTACHMENT17, GL_COLOR_ATTACHMENT18, GL_COLOR_ATTACHMENT19, 
                               GL_COLOR_ATTACHMENT20, GL_COLOR_ATTACHMENT21, GL_COLOR_ATTACHMENT22, GL_COLOR_ATTACHMENT23, 
                               GL_COLOR_ATTACHMENT24, GL_COLOR_ATTACHMENT25, GL_COLOR_ATTACHMENT26, GL_COLOR_ATTACHMENT27, 
                               GL_COLOR_ATTACHMENT28, GL_COLOR_ATTACHMENT29, GL_COLOR_ATTACHMENT30, GL_COLOR_ATTACHMENT31
                            };
            
            glDrawBuffers(m_colorBufferAttachments.size(), buffer);

        }
        else if(m_colorBufferAttachments.empty()){
            // Only Depth Pass
            glDrawBuffer(GL_NONE);
        }
        
        if(glCheckNamedFramebufferStatus(m_rendererID ,GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE){
            ENGINE_LOG_ERROR("Create Frame Buffer!!!");
        }

        // Unbind
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) noexcept {
        ENGINE_ASSERT((attachmentIndex < m_colorBufferAttachments.size()), "Attachment index > 32");

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData{};
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

        return pixelData;
    }

    void OpenGLFrameBuffer::ClearColorBufferAttachment(uint32_t attachmentIndex, int value) noexcept {

        ENGINE_ASSERT((attachmentIndex < m_colorBufferAttachments.size()), "Attachment index > 32");

        auto& spec = m_colorBufferAttachmentsSpec[attachmentIndex];

        glClearTexImage(m_colorBufferAttachments[attachmentIndex], 0, 
            ConvertTextureFormatToOpenGLFormat(spec.TextureFormat), GL_INT, &value);

    }

}