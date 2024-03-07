#pragma once
#include <cstdint>
#include <CreepyEngine/Core/Core.hpp>

namespace Creepy
{
    class Texture
    {
        public:
            virtual ~Texture() noexcept = default;

            virtual uint32_t GetWidth() const noexcept = 0;
            virtual uint32_t GetHeight() const noexcept = 0;
            virtual uint32_t GetRendererID() const noexcept = 0;

            virtual void Bind(uint32_t slot = 0) const noexcept = 0;
            virtual void UnBind() const noexcept = 0;

            virtual void SetData(void* data, uint32_t size) noexcept = 0;


            virtual bool operator==(const Texture& other) const noexcept = 0;
           
    };
    

    class Texture2D : public Texture
    {
        public:
            virtual ~Texture2D() noexcept = default;

            static Ref<Texture2D> Create(uint32_t width, uint32_t height) noexcept;

            static Ref<Texture2D> Create(const std::string& filePath) noexcept;

            

        private:

    };
    
}
