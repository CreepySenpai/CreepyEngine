#include <Platform/OpenGL/OpenGLVertexArray.hpp>
#include <glad/glad.h>

namespace Creepy
{

    static GLenum ShaderDataTypeToOpenGLDataType(ShaderDataType dataType) noexcept
    {
        switch (dataType)
        {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
                return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
                return GL_INT;
            case ShaderDataType::Bool:
            {
                return GL_BOOL;
            }
        }

        ENGINE_LOG_ERROR("Error Invalid Data Type!!!");

        std::unreachable();
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray() noexcept {
        glCreateVertexArrays(1, &m_renderID);
    }

    OpenGLVertexArray::~OpenGLVertexArray() noexcept {
        glDeleteVertexArrays(1, &m_renderID);
    }

    void OpenGLVertexArray::Bind() const noexcept {
        glBindVertexArray(m_renderID);
    }

    void OpenGLVertexArray::UnBind() const noexcept {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) noexcept {
        if(vertexBuffer->GetLayout().GetElements().size() == 0){
            ENGINE_LOG_ERROR("VertexBuffer Has No Layout!!!");
            return;
        }

        glBindVertexArray(m_renderID);
        vertexBuffer->Bind();   // We bind vertex buffer data to vertex array 

        const auto& layout = vertexBuffer->GetLayout(); // We get all layout in vertex array
        
        //Enable and set up data for every vertex attrib
        for(uint32_t index{0}; const auto& element : layout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLDataType(element.Type), 
            element.Normalize ? GL_TRUE : GL_FALSE, layout.GetStride(), reinterpret_cast<const void*>(element.Offset));
            ++index;
        }

        m_vertexBuffers.emplace_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) noexcept {
        if(indexBuffer->GetCount() == 0){
            ENGINE_LOG_ERROR("IndexBuffer Has No Setup!!!");
            return;
        }

        glBindVertexArray(m_renderID);
        indexBuffer->Bind();

        m_indexBuffer = indexBuffer;
    }
}
