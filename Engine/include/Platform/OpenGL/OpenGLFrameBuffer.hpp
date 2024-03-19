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

            void Resize(uint32_t width, uint32_t height) noexcept override;

            inline uint32_t GetFrameBufferID() const noexcept override {
                return m_rendererID;
            }

            inline uint32_t GetColorAttachmentID(uint32_t index = 0) const noexcept override {
                return m_colorBufferAttachments[index];
            }

            inline FrameBufferSpecification& GetSpecification() noexcept override {
                return m_data;
            }

            inline const FrameBufferSpecification& GetSpecification() const noexcept override {
                return m_data;
            }
        private:
            uint32_t m_rendererID{};
            FrameBufferSpecification m_data;
            std::vector<FrameBufferTextureSpecification> m_colorBufferAttachmentsSpec;
            FrameBufferTextureSpecification m_deptBufferAttachmentSpec;
            std::vector<uint32_t> m_colorBufferAttachments;
            uint32_t m_depthBufferAttachment{0};
    };
    
}
