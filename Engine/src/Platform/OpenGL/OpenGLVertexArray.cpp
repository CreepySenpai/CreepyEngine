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
        glCreateVertexArrays(1, &m_rendererID);
        ENGINE_LOG_WARNING("Create A VertexArray: {}", m_rendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray() noexcept {
        glDeleteVertexArrays(1, &m_rendererID);
        ENGINE_LOG_WARNING("Delete A VertexArray: {}", m_rendererID);
    }

    void OpenGLVertexArray::Bind() const noexcept {
        glBindVertexArray(m_rendererID);
    }

    void OpenGLVertexArray::UnBind() const noexcept {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) noexcept {
        if(vertexBuffer->GetLayout().GetElements().size() == 0){
            ENGINE_LOG_ERROR("VertexBuffer Has No Layout!!!");
            return;
        }

        glBindVertexArray(m_rendererID);
        vertexBuffer->Bind();   // We bind vertex buffer data to vertex array 

        const auto& layout = vertexBuffer->GetLayout(); // We get all layout in vertex array
        
        //Enable and set up data for every vertex attrib
        for(const auto& element : layout) {
            switch(element.Type){
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4: {
                    glEnableVertexAttribArray(m_vertexBufferIndex);

                    glVertexAttribPointer(m_vertexBufferIndex, element.GetComponentCount(), ShaderDataTypeToOpenGLDataType(element.Type), 
                        element.Normalize ? GL_TRUE : GL_FALSE, layout.GetStride(), reinterpret_cast<const void*>(element.Offset));
                        
                    ++m_vertexBufferIndex;
                    break;
                }

                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4: {
                    uint32_t count = element.GetComponentCount();
                    for(uint32_t i{}; i < count; i++){
                        glEnableVertexAttribArray(m_vertexBufferIndex);

                        glVertexAttribPointer(m_vertexBufferIndex, count, ShaderDataTypeToOpenGLDataType(element.Type), 
                        element.Normalize ? GL_TRUE : GL_FALSE, layout.GetStride(), reinterpret_cast<const void*>(element.Offset + sizeof(float) * count * i));
                        
                        glVertexAttribDivisor(m_vertexBufferIndex, 1);
                        ++m_vertexBufferIndex;
                    }
                    
                    break;
                }

                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool: {
                    glEnableVertexAttribArray(m_vertexBufferIndex);

                    glVertexAttribIPointer(m_vertexBufferIndex, element.GetComponentCount(), ShaderDataTypeToOpenGLDataType(element.Type), 
                        layout.GetStride(), reinterpret_cast<const void*>(element.Offset));

                    ++m_vertexBufferIndex;
                    break;
                }
            }
        }

        m_vertexBuffers.emplace_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) noexcept {
        if(indexBuffer->GetCount() == 0){
            ENGINE_LOG_ERROR("IndexBuffer Has No Setup!!!");
            return;
        }

        glBindVertexArray(m_rendererID);
        indexBuffer->Bind();

        m_indexBuffer = indexBuffer;
    }
}
