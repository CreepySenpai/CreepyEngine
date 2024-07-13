#pragma once

#include <CreepyEngine/Renderer/UniformBuffer.hpp>

namespace Creepy {

    class OpenGLUniformBuffer2 : public UniformBuffer2 {
        friend class UniformBuffer2;

        public:
            OpenGLUniformBuffer2(uint32_t size, uint32_t binding) noexcept;
            ~OpenGLUniformBuffer2() noexcept;

        private:
            void SetDataImpl(const void* data, uint32_t size, uint32_t offset = 0) noexcept;
            
        private:
            uint32_t m_rendererID;
    };

}