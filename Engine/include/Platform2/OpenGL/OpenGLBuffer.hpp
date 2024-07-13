#pragma once

#include <CreepyEngine/Renderer/Buffer.hpp>


namespace Creepy {

    class OpenGLVertexBuffer2 : public VertexBuffer2{
        friend class VertexBuffer2;

        public:
            OpenGLVertexBuffer2() noexcept = default;
            OpenGLVertexBuffer2(uint32_t size) noexcept;
            OpenGLVertexBuffer2(const void* data, uint32_t size) noexcept;

            ~OpenGLVertexBuffer2() noexcept;

        private:
            void BindImpl() const noexcept;

            void UnBindImpl() const noexcept;
            
            const BufferLayout& GetLayoutImpl() const noexcept {
                return m_layout;
            }

            inline void SetLayoutImpl(const BufferLayout& layout) noexcept {
                m_layout = layout;
            }
            
            void SetDataImpl(const void* data, uint32_t size) noexcept;

        private:
            uint32_t m_rendererID;
            BufferLayout m_layout;
    };

    class OpenGLIndexBuffer2 : public IndexBuffer2 {
        friend class IndexBuffer2;
        
        public:
            OpenGLIndexBuffer2() noexcept = default;
            OpenGLIndexBuffer2(const void* indexData, uint32_t count) noexcept;
            
            ~OpenGLIndexBuffer2() noexcept;
        private:
            void BindImpl() const noexcept;

            void UnBindImpl() const noexcept;

            uint32_t GetCountImpl() const noexcept {
                return m_count;
            }

        private:
            uint32_t m_rendererID;
            uint32_t m_count;
    };

}