#pragma once

#include <CreepyEngine/Renderer/Buffer.hpp>

namespace Creepy {

    class OpenGLVertexBuffer : public VertexBuffer {
        public:
            OpenGLVertexBuffer(uint32_t size) noexcept;
            OpenGLVertexBuffer(float* vertex, uint32_t size) noexcept;
            virtual ~OpenGLVertexBuffer() noexcept;
            void Bind() const noexcept override;

            void UnBind() const noexcept override;

            inline const BufferLayout& GetLayout() const noexcept override {
                return m_layout;
            }

            inline void SetLayout(const BufferLayout& layout) noexcept override {
                m_layout = layout;
            }

            void SetData(const void* data, uint32_t size) noexcept override;

        private:
            uint32_t m_rendererID;
            BufferLayout m_layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer {
        public:
            OpenGLIndexBuffer(uint32_t* index, uint32_t count) noexcept;
            virtual ~OpenGLIndexBuffer() noexcept;
            void Bind() const noexcept override;

            void UnBind() const noexcept override;

            uint32_t GetCount() const noexcept override;

        private:
            uint32_t m_rendererID;
            uint32_t m_count;
    };
}