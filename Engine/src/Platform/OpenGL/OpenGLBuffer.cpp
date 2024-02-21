#include <Platform/OpenGL/OpenGLBuffer.hpp>
#include <glad/glad.h>

namespace Creepy
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertex, uint32_t size) noexcept {
        glCreateBuffers(1, &m_rendererID);

        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);

        glBufferData(GL_ARRAY_BUFFER, size, vertex, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() noexcept{
        glDeleteBuffers(1, &m_rendererID);
    }

    void OpenGLVertexBuffer::Bind() const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    }

    void OpenGLVertexBuffer::UnBind() const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* index, uint32_t count) noexcept : m_count{count} {
        glCreateBuffers(1, &m_rendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), index, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() noexcept {
        glDeleteBuffers(1, &m_rendererID);
    }

    void OpenGLIndexBuffer::Bind() const noexcept {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
    }

    void OpenGLIndexBuffer::UnBind() const noexcept {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    uint32_t OpenGLIndexBuffer::GetCount() const noexcept {
        return m_count;
    }
}
