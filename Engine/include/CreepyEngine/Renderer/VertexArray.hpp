#pragma once

#include <memory>
#include <CreepyEngine/Core/Core.hpp>
#include "Buffer.hpp"

namespace Creepy
{
    class VertexArray
    {
        public:
            virtual ~VertexArray() noexcept = default;

            virtual void Bind() const noexcept = 0;

            virtual void UnBind() const noexcept = 0;

            virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) noexcept = 0;

            virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) noexcept = 0;

            virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const noexcept = 0;

            virtual const Ref<IndexBuffer>& GetIndexBuffer() const noexcept = 0;

            static Ref<VertexArray> Create() noexcept;
    };
}
