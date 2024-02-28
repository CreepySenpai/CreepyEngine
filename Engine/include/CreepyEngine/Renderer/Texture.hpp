#pragma once
#include <cstdint>
#include <CreepyEngine/Core/Core.hpp>

namespace Creepy
{
    class Texture
    {
        public:
            
            virtual uint32_t GetWidth() const noexcept = 0;
            virtual uint32_t GetHeight() const noexcept = 0;

            virtual void Bind(uint32_t slot = 0) const noexcept = 0;

            virtual ~Texture() noexcept = default;
           
    };
    

    class Texture2D : public Texture
    {
        public:
            virtual ~Texture2D() noexcept = default;

            static Ref<Texture2D> Create(const std::string& filePath) noexcept;

        private:

    };
    
}
