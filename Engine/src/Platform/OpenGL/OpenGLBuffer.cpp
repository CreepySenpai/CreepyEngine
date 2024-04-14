#include <Platform/OpenGL/OpenGLBuffer.hpp>
#include <glad/glad.h>

namespace Creepy
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) noexcept {
        glCreateBuffers(1, &m_rendererID);

        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);

        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

        ENGINE_LOG_WARNING("Create A VertexBuffer: {}", m_rendererID);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, uint32_t size) noexcept {
        glCreateBuffers(1, &m_rendererID);

        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);

        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

        ENGINE_LOG_WARNING("Create A VertexBuffer: {}", m_rendererID);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() noexcept{
        ENGINE_LOG_WARNING("Delete A VertexBuffer: {}", m_rendererID);

        glDeleteBuffers(1, &m_rendererID);
    }

    void OpenGLVertexBuffer::Bind() const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    }

    void OpenGLVertexBuffer::UnBind() const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    ////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    
    OpenGLIndexBuffer::OpenGLIndexBuffer(const void* indexData, uint32_t count) noexcept : m_count{count} {
        glCreateBuffers(1, &m_rendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indexData, GL_STATIC_DRAW);

        ENGINE_LOG_WARNING("Create A IndexBuffer: {}", m_rendererID);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() noexcept {
        ENGINE_LOG_WARNING("Delete A IndexBuffer: {}", m_rendererID);
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
