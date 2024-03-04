#pragma once

#include <CreepyEngine/Renderer/Texture.hpp>
#include <glad/glad.h>

namespace Creepy {
    class OpenGLTexture2D : public Texture2D
    {
        public:
            OpenGLTexture2D(const std::string& filePath) noexcept;

            OpenGLTexture2D(uint32_t width, uint32_t height) noexcept;

            ~OpenGLTexture2D() noexcept;
            
            constexpr inline uint32_t GetWidth() const noexcept override {
                return m_width;
            }

            constexpr inline uint32_t GetHeight() const noexcept override {
                return m_height;
            }

            constexpr inline uint32_t GetRendererID() const noexcept override {
                return m_rendererID;
            }

            void Bind(uint32_t slot = 0) const noexcept override;

            void UnBind() const noexcept override;

            void SetData(void* data, uint32_t size) noexcept override;

            bool operator==(const Texture& other) const noexcept override {
                return m_rendererID == other.GetRendererID();
            }
            

        private:
            uint32_t m_width;
            uint32_t m_height;
            uint32_t m_rendererID;
            GLenum m_internalFormat, m_dataFormat;
            std::string m_path;
    };
}