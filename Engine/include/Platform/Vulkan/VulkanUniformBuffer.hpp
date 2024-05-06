#pragma once

#include <CreepyEngine/Renderer/UniformBuffer.hpp>
#include "VulkanBuffer.hpp"

namespace Creepy {

    class VulkanUniformBuffer : public UniformBuffer {
        
        public:
            VulkanUniformBuffer(uint32_t size) noexcept;

            void Bind() noexcept;

            void Destroy() noexcept;

            void SetData(const void* data, uint32_t size, uint32_t offset = 0) noexcept override;
            
        private:
            VulkanBuffer m_buffer;
    };

}