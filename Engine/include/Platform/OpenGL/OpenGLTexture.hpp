#pragma once

#include <CreepyEngine/Renderer/Texture.hpp>

namespace Creepy {
    class OpenGLTexture2D : public Texture2D
    {
        public:
            OpenGLTexture2D(const std::string& filePath) noexcept;

            ~OpenGLTexture2D() noexcept;
            
            constexpr inline uint32_t GetWidth() const noexcept override {
                return m_width;
            }

            constexpr inline uint32_t GetHeight() const noexcept override {
                return m_height;
            }

            void Bind(uint32_t slot = 0) const noexcept override;
        private:
            uint32_t m_width;
            uint32_t m_height;
            uint32_t m_rendererID;
            std::string m_path;
    };
}