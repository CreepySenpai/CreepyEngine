#pragma once

#include <CreepyEngine/Core/Core.hpp>

namespace Creepy {

    struct FrameBufferSpecification{
        uint32_t Width;
        uint32_t Height;
        uint32_t Samples{1};
        bool SwapChainTarget{false};
    };

    class FrameBuffer
    {
        public:
            virtual ~FrameBuffer() noexcept = default;

            virtual void Bind() noexcept = 0;

            virtual void UnBind() noexcept = 0;

            virtual uint32_t GetFrameBufferID() const noexcept = 0;

            virtual uint32_t GetColorAttachmentID() const noexcept = 0;

            virtual FrameBufferSpecification& GetSpecification() noexcept = 0;

            virtual const FrameBufferSpecification& GetSpecification() const noexcept = 0;

            static Ref<FrameBuffer> Create(const FrameBufferSpecification& data) noexcept;
        private:
            
    };
}