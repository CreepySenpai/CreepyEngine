#include <Platform2/OpenGL/OpenGLUniformBuffer.hpp>
#include <glad/glad.h>

namespace Creepy {

    OpenGLUniformBuffer2::OpenGLUniformBuffer2(uint32_t size, uint32_t binding) noexcept {
        glCreateBuffers(1, &m_rendererID);
        glNamedBufferData(m_rendererID, size, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_rendererID);
    }

    OpenGLUniformBuffer2::~OpenGLUniformBuffer2() noexcept {
        glDeleteBuffers(1, &m_rendererID);
    }

    void OpenGLUniformBuffer2::SetDataImpl(const void* data, uint32_t size, uint32_t offset) noexcept {
        glNamedBufferSubData(m_rendererID, offset, size, data);
    }
}