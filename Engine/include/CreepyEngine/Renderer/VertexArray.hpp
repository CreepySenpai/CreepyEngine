#pragma once

#include <memory>
#include "Buffer.hpp"

namespace Creepy
{
    class VertexArray
    {
        public:
            virtual ~VertexArray() noexcept = default;

            virtual void Bind() const noexcept = 0;

            virtual void UnBind() const noexcept = 0;

            virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) noexcept = 0;

            virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) noexcept = 0;

            virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const noexcept = 0;

            virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const noexcept = 0;

            static VertexArray* Create() noexcept;
    };
}
