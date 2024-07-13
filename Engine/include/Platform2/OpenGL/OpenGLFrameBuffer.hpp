#pragma once

#include <CreepyEngine/Renderer/FrameBuffer.hpp>

namespace Creepy{

    class OpenGLFrameBuffer2 : public FrameBuffer2{
        friend class FrameBuffer2;
        
        public:
            OpenGLFrameBuffer2(const FrameBufferSpecification& data) noexcept;
            ~OpenGLFrameBuffer2() noexcept;
            
        private:
            void InvalidateImpl() noexcept;

            void BindImpl() noexcept;

            void UnBindImpl() noexcept;

            void ResizeImpl(uint32_t width, uint32_t height) noexcept;

            inline uint32_t GetFrameBufferIDImpl() const noexcept {
                return m_rendererID;
            }

            inline uint32_t GetColorAttachmentIDImpl(uint32_t index = 0) const noexcept {
                return m_colorBufferAttachments[index];
            }

            int ReadPixelImpl(uint32_t attachmentIndex, int x, int y) noexcept;

            void ClearColorBufferAttachmentImpl(uint32_t attachmentIndex, int value) noexcept;

            inline FrameBufferSpecification& GetSpecificationImpl() noexcept {
                return m_data;
            }

            inline const FrameBufferSpecification& GetSpecificationImpl() const noexcept {
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
