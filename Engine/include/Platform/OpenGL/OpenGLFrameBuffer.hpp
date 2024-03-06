#pragma once

#include <CreepyEngine/Renderer/FrameBuffer.hpp>


namespace Creepy
{
    
    class OpenGLFrameBuffer : public FrameBuffer
    {
        public:
            OpenGLFrameBuffer(const FrameBufferSpecification& data) noexcept;
            virtual ~OpenGLFrameBuffer() noexcept;
            void Invalidate() noexcept;

            void Bind() noexcept override;

            void UnBind() noexcept override;

            inline uint32_t GetFrameBufferID() const noexcept override {
                return m_rendererID;
            }

            inline uint32_t GetColorAttachmentID() const noexcept override {
                return m_colorBuffer;
            }

            inline FrameBufferSpecification& GetSpecification() noexcept override {
                return m_data;
            }

            inline const FrameBufferSpecification& GetSpecification() const noexcept override {
                return m_data;
            }
        private:
            uint32_t m_rendererID;
            uint32_t m_colorBuffer;
            uint32_t m_depthBuffer;
            FrameBufferSpecification m_data;
    };
    
}
