#pragma once

#include <CreepyEngine/Core/Core.hpp>

namespace Creepy {

    enum class FrameBufferTextureFormat : uint32_t{
        NONE    = 0,

        // Color
        RGBA8, RED_INT, GREEN_INT, BLUE_INT,
        RGBA16_SFLOAT, RGBA16_SINT, RGBA16_UINT,
        // Depth, Stencil
        DEPTH32,
        DEPTH24STENCIL8,

        DEPTH = DEPTH24STENCIL8,
    };

    struct FrameBufferTextureSpecification{
        FrameBufferTextureFormat TextureFormat {FrameBufferTextureFormat::NONE};

        constexpr FrameBufferTextureSpecification() noexcept = default;
        
        constexpr FrameBufferTextureSpecification(FrameBufferTextureFormat format) noexcept : TextureFormat{format}{

        }

    };

    struct FrameBufferAttachmentSpecification{
        std::vector<FrameBufferTextureSpecification> AttachmentList;

        constexpr FrameBufferAttachmentSpecification() noexcept = default;

        constexpr FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachMents) noexcept 
            : AttachmentList{attachMents} {

        }


    };

    struct FrameBufferSpecification{
        uint32_t Width;
        uint32_t Height;
        uint32_t Samples{1};
        FrameBufferAttachmentSpecification Attachments;
        bool SwapChainTarget{false};
    };

    class FrameBuffer
    {
        public:
            virtual ~FrameBuffer() noexcept = default;

            virtual void Bind() noexcept = 0;

            virtual void UnBind() noexcept = 0;

            virtual void Resize(uint32_t width, uint32_t height) noexcept = 0;

            virtual uint32_t GetFrameBufferID() const noexcept = 0;

            virtual uint32_t GetColorAttachmentID(uint32_t index = 0) const noexcept = 0;

            virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) noexcept = 0;

            virtual void ClearColorBufferAttachment(uint32_t attachmentIndex, int value) noexcept = 0;

            virtual FrameBufferSpecification& GetSpecification() noexcept = 0;

            virtual const FrameBufferSpecification& GetSpecification() const noexcept = 0;

            static Ref<FrameBuffer> Create(const FrameBufferSpecification& data) noexcept;
        private:
            
    };

    class FrameBuffer2 {
        public:
            void Bind(this auto&& self) noexcept {
                self.BindImpl();
            }

            void UnBind(this auto&& self) noexcept {
                self.UnBindImpl();
            }

            void Resize(this auto&& self, uint32_t width, uint32_t height) noexcept {
                self.ResizeImpl(width, height);
            }

            uint32_t GetFrameBufferID(this auto&& self) noexcept {
                return self.GetFrameBufferIDImpl();
            }

            uint32_t GetColorAttachmentID(this auto&& self, uint32_t index = 0) noexcept {
                return self.GetColorAttachmentIDImpl(index);
            }

            int ReadPixel(this auto&& self, uint32_t attachmentIndex, int x, int y) noexcept {
                return self.ReadPixelImpl(attachmentIndex, x, y);
            }

            void ClearColorBufferAttachment(this auto&& self, uint32_t attachmentIndex, int value) noexcept {
                self.ClearColorBufferAttachmentImpl(attachmentIndex, value);
            }

            FrameBufferSpecification& GetSpecification(this auto&& self) noexcept {
                return self.GetSpecificationImpl();
            }
    };
}