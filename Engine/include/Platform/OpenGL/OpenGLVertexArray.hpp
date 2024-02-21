#pragma once

#include <CreepyEngine/Renderer/VertexArray.hpp>
#include <CreepyEngine/Renderer/Buffer.hpp>

namespace Creepy
{
    class OpenGLVertexArray : public VertexArray
    {
        public:
            OpenGLVertexArray() noexcept;
            ~OpenGLVertexArray() noexcept;

            void Bind() const noexcept override;

            void UnBind() const noexcept override;

            void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) noexcept override;

            void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) noexcept override;

            inline const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const noexcept override {
                return m_vertexBuffers;
            }

            inline const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const noexcept override {
                return m_indexBuffer;
            }

        private:
            uint32_t m_renderID;
            std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
            std::shared_ptr<IndexBuffer> m_indexBuffer;
    };
    
}
