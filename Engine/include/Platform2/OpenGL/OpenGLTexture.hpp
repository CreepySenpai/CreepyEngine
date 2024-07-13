#pragma once

#include <CreepyEngine/Renderer/Texture.hpp>


namespace Creepy {

    class OpenGLTexture2D2 : public Texture2D2 {
        using GLenum = unsigned int;

        friend class Texture2;
        friend class Texture2D2;

        public:
            OpenGLTexture2D2(const std::filesystem::path& filePath) noexcept;

            OpenGLTexture2D2(uint32_t width, uint32_t height) noexcept;

            ~OpenGLTexture2D2() noexcept;
            
            constexpr inline uint32_t GetWidthImpl() const noexcept {
                return m_width;
            }

            constexpr inline uint32_t GetHeightImpl() const noexcept {
                return m_height;
            }

            constexpr inline uint32_t GetRendererIDImpl() const noexcept {
                return m_rendererID;
            }

            const std::filesystem::path& GetTexturePathImpl() const noexcept {
                return m_filePath;
            }

            void BindImpl(uint32_t slot = 0) const noexcept;

            void UnBindImpl() const noexcept;

            void SetDataImpl(void* data, uint32_t size) noexcept;

            bool operator==(const OpenGLTexture2D2& other) const noexcept {
                return m_rendererID == other.GetRendererID();
            }
        private:
            uint32_t m_width;
            uint32_t m_height;
            uint32_t m_rendererID;
            GLenum m_internalFormat, m_dataFormat;
            std::filesystem::path m_filePath;
    };

}