#include <Platform/OpenGL/OpenGLTexture.hpp>
#include <stb_image/stb_image.hpp>
#include <glad/glad.h>

namespace Creepy {

    OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath) noexcept : m_path{filePath} {
        stbi_set_flip_vertically_on_load(1);
        int width, height, channels;

        auto imgData = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

        if(!imgData){
            ENGINE_LOG_ERROR("Faile to load image {}!", filePath);
        }

        m_width = static_cast<uint32_t>(width);
        m_height = static_cast<uint32_t>(height);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
        
        glTextureStorage2D(m_rendererID, 1, GL_RGB8, m_width, m_height);
        
        glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, imgData);

        stbi_image_free(imgData);

    }

    OpenGLTexture2D::~OpenGLTexture2D() noexcept {
        glDeleteTextures(1, &m_rendererID);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const noexcept {
        glBindTextureUnit(slot, m_rendererID);
    }
    
}