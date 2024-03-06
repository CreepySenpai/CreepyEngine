#include <Platform/OpenGL/OpenGLFrameBuffer.hpp>
#include <CreepyEngine/Debug/ErrorHandle.hpp>
#include <glad/glad.h>


namespace Creepy {

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& data) noexcept : m_data{data} {

        Invalidate();

    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer() noexcept {
        OPENGL_CHECK_ERROR(glDeleteRenderbuffers(1, &m_depthBuffer));
        OPENGL_CHECK_ERROR(glDeleteFramebuffers(1, &m_rendererID));

    }

    void OpenGLFrameBuffer::Bind() noexcept {
        OPENGL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));
    }

    void OpenGLFrameBuffer::UnBind() noexcept {

        OPENGL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        
    }

    void OpenGLFrameBuffer::Invalidate() noexcept {

        ENGINE_LOG_WARNING("W: {}, H: {}", m_data.Width, m_data.Height);

        OPENGL_CHECK_ERROR(glCreateFramebuffers(1, &m_rendererID));

        OPENGL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));


        // Create Color Buffer
        OPENGL_CHECK_ERROR(glCreateTextures(GL_TEXTURE_2D, 1, &m_colorBuffer));
        OPENGL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, m_colorBuffer));

        OPENGL_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_data.Width, m_data.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

        OPENGL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		OPENGL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		OPENGL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		OPENGL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		OPENGL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        OPENGL_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0));

        // Test Create depth buffer
        OPENGL_CHECK_ERROR(glCreateRenderbuffers(1, &m_depthBuffer));
        OPENGL_CHECK_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer));
        OPENGL_CHECK_ERROR(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_data.Width, m_data.Height));
        OPENGL_CHECK_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer));



        // Create Depth, Stencil Buffer
        // OPENGL_CHECK_ERROR(glCreateTextures(GL_TEXTURE_2D, 1, &m_depthBuffer));
        // OPENGL_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, m_depthBuffer));

        // OPENGL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		// OPENGL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		// OPENGL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		// OPENGL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		// OPENGL_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        // First 24 bit for depth buffer, 8 bit remain for stencil buffer, level must > 0
        // glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_data.Width, m_data.Height);
        // OPENGL_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_data.Width, m_data.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));

        // OPENGL_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer, 0));
        
        if(glCheckNamedFramebufferStatus(m_rendererID ,GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE){
            ENGINE_LOG_ERROR("Create Frame Buffer!!!");
        }

        // Unbind
        OPENGL_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

}