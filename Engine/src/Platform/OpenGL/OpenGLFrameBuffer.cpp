#include <Platform/OpenGL/OpenGLFrameBuffer.hpp>
#include <CreepyEngine/Debug/ErrorHandle.hpp>
#include <glad/glad.h>


namespace Creepy {

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& data) noexcept : m_data{data} {

        this->Invalidate();

    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer() noexcept {

        glDeleteFramebuffers(1, &m_rendererID);
        glDeleteTextures(1, &m_colorBuffer);
        glDeleteTextures(1, &m_depthBuffer);
        
    }

    void OpenGLFrameBuffer::Bind() noexcept {
        glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
        glViewport(0, 0, m_data.Width, m_data.Height);
    }

    void OpenGLFrameBuffer::UnBind() noexcept {
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

    void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height) noexcept {

        m_data.Width = width;
        m_data.Height = height;

        this->Invalidate();

    }

    void OpenGLFrameBuffer::Invalidate() noexcept {
        
        // Check If Frame Buffer Already Create
        if(m_rendererID){
            glDeleteFramebuffers(1, &m_rendererID);
            glDeleteTextures(1, &m_colorBuffer);
            glDeleteTextures(1, &m_depthBuffer);
        }

        glCreateFramebuffers(1, &m_rendererID);

        glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);


        // Create Color Buffer
        glCreateTextures(GL_TEXTURE_2D, 1, &m_colorBuffer);
        glBindTexture(GL_TEXTURE_2D, m_colorBuffer);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_data.Width, m_data.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);


        // We should prefer texture buffer then render buffer
        // Create Depth, Stencil Buffer
        glCreateTextures(GL_TEXTURE_2D, 1, &m_depthBuffer);
        glBindTexture(GL_TEXTURE_2D, m_depthBuffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // First 24 bit for depth buffer, 8 bit remain for stencil buffer, level must > 0
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_data.Width, m_data.Height);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer, 0);
        
        if(glCheckNamedFramebufferStatus(m_rendererID ,GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE){
            ENGINE_LOG_ERROR("Create Frame Buffer!!!");
        }

        // Unbind
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

}