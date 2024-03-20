#pragma once

#include <CreepyEngine/Renderer/UniformBuffer.hpp>


namespace Creepy{

    class OpenGLUniformBuffer : public UniformBuffer
    {
        public:
            OpenGLUniformBuffer(uint32_t size, uint32_t binding) noexcept;
            virtual ~OpenGLUniformBuffer() noexcept;

            void SetData(const void* data, uint32_t size, uint32_t offset = 0) noexcept override;
        private:
            uint32_t m_rendererID;
    };
}