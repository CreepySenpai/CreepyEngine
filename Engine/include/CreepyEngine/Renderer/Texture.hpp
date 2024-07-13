#pragma once
#include <cstdint>
#include <CreepyEngine/Core/Core.hpp>
#include <filesystem>

namespace Creepy
{
    class Texture
    {
        public:
            virtual ~Texture() noexcept = default;

            virtual uint32_t GetWidth() const noexcept = 0;
            virtual uint32_t GetHeight() const noexcept = 0;
            virtual uint32_t GetRendererID() const noexcept = 0;
            [[nodiscard]] virtual const std::filesystem::path& GetTexturePath() const noexcept = 0;

            virtual void Bind(uint32_t slot = 0) const noexcept = 0;
            virtual void UnBind() const noexcept = 0;

            virtual void SetData(void* data, uint32_t size) noexcept = 0;

            virtual bool operator==(const Texture& other) const noexcept = 0;
    };
    
    class Texture2{
        public:
            uint32_t GetWidth(this auto&& self) noexcept {
                return self.GetWidthImpl();
            }
            uint32_t GetHeight(this auto&& self) noexcept {
                return self.GetHeightImpl();
            }

            uint32_t GetRendererID(this auto&& self) noexcept {
                return self.GetRendererIDImpl();
            }

            [[nodiscard]] const std::filesystem::path& GetTexturePath(this auto&& self) noexcept {
                return self.GetTexturePathImpl();
            }

            void Bind(this auto&& self, uint32_t slot = 0) noexcept {
                self.BindImpl(slot);
            }
            
            void UnBind(this auto&& self) noexcept {
                self.UnBindImpl();
            }

            void SetData(this auto&& self, void* data, uint32_t size) noexcept {
                self.SetDataImpl(size);
            }
    };

    class Texture2D2 : public Texture2{};

    class Texture2D : public Texture
    {
        public:
            virtual ~Texture2D() noexcept = default;

            [[nodiscard]] static Ref<Texture2D> Create(uint32_t width, uint32_t height) noexcept;

            [[nodiscard]] static Ref<Texture2D> Create(const std::filesystem::path& filePath) noexcept;

            

        private:

    };
    
}
