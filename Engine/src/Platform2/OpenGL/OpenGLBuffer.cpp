#include <Platform2/OpenGL/OpenGLBuffer.hpp>
#include <glad/glad.h>

namespace Creepy{

    OpenGLVertexBuffer2::OpenGLVertexBuffer2(uint32_t size) noexcept
    {
        glCreateBuffers(1, &m_rendererID);

        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);

        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

        ENGINE_LOG_WARNING("Create A VertexBuffer: {}", m_rendererID);
    }

    OpenGLVertexBuffer2::OpenGLVertexBuffer2(const void *data, uint32_t size) noexcept
    {
        glCreateBuffers(1, &m_rendererID);

        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);

        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

        ENGINE_LOG_WARNING("Create A VertexBuffer: {}", m_rendererID);
    }

    OpenGLVertexBuffer2::~OpenGLVertexBuffer2() noexcept {
        ENGINE_LOG_WARNING("Delete A VertexBuffer: {}", m_rendererID);

        glDeleteBuffers(1, &m_rendererID);
    }

    void OpenGLVertexBuffer2::BindImpl() const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    }

    void OpenGLVertexBuffer2::UnBindImpl() const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer2::SetDataImpl(const void* data, uint32_t size) noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }


    //////////////////////////////////////////

    OpenGLIndexBuffer2::OpenGLIndexBuffer2(const void* indexData, uint32_t count) noexcept {
        glCreateBuffers(1, &m_rendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indexData, GL_STATIC_DRAW);

        ENGINE_LOG_WARNING("Create A IndexBuffer: {}", m_rendererID);
    }

    OpenGLIndexBuffer2::~OpenGLIndexBuffer2() noexcept {
        ENGINE_LOG_WARNING("Delete A IndexBuffer: {}", m_rendererID);
        glDeleteBuffers(1, &m_rendererID);
    }

    void OpenGLIndexBuffer2::BindImpl() const noexcept {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
    }

    void OpenGLIndexBuffer2::UnBindImpl() const noexcept {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}