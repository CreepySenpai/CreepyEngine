#pragma once

#include <cstdint>
#include <CreepyEngine/Core/Core.hpp>

namespace Creepy {

    class UniformBuffer
    {
        public:
            virtual ~UniformBuffer() noexcept = default;

            virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) noexcept = 0;

            [[nodiscard]] static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding) noexcept;            
    };


    class UniformBuffer2 {
        public:
            void SetData(this auto&& self, const void* data, uint32_t size, uint32_t offset = 0) noexcept {
                self.SetDataImpl(data, size, offset);
            }
    };
}