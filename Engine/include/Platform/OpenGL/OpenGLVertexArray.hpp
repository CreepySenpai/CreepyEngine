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

            void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) noexcept override;

            void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) noexcept override;

            inline const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const noexcept override {
                return m_vertexBuffers;
            }

            inline const Ref<IndexBuffer>& GetIndexBuffer() const noexcept override {
                return m_indexBuffer;
            }

        private:
            uint32_t m_renderID;
            std::vector<Ref<VertexBuffer>> m_vertexBuffers;
            Ref<IndexBuffer> m_indexBuffer;
    };
    
}
