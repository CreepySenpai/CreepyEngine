#include <Platform/OpenGL/OpenGLTexture.hpp>
#include <stb_image/stb_image.hpp>
#include <CreepyEngine/Project/Project.hpp>
namespace Creepy {

    OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& filePath) noexcept : m_filePath{filePath} {
        stbi_set_flip_vertically_on_load(1);
        int width, height, channels;
        std::string path = filePath.string();
        auto imgData = stbi_load(path.c_str(), &width, &height, &channels, 0);
        
        ENGINE_ASSERT((imgData != nullptr), std::format("Failed to load image: {}", filePath.string()));

        m_width = static_cast<uint32_t>(width);
        m_height = static_cast<uint32_t>(height);

        if(channels == 4){      // 4 channels rgba
            m_internalFormat = GL_RGBA8;
            m_dataFormat = GL_RGBA;
        }
        else if(channels == 3) {
            m_internalFormat = GL_RGB8;
            m_dataFormat = GL_RGB;
        } else {
            ENGINE_LOG_ERROR("Current texture format doesn't support!");
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
        
        glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);
        
        glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, imgData);

        stbi_image_free(imgData);

        ENGINE_LOG_WARNING("Create A Texture: {}", m_rendererID);

    }

    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) noexcept : 
        m_width{width}, m_height{height}, m_internalFormat{GL_RGBA8}, m_dataFormat{GL_RGBA} {
        
        glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
        
        glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);
        
        glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        ENGINE_LOG_WARNING("Create A Texture: {}", m_rendererID);

    }

    OpenGLTexture2D::~OpenGLTexture2D() noexcept {
        ENGINE_LOG_WARNING("Delete A Texture: {}", m_rendererID);
        glDeleteTextures(1, &m_rendererID);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const noexcept {
        glBindTextureUnit(slot, m_rendererID);
    }

    void OpenGLTexture2D::UnBind() const noexcept {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size) noexcept {
        uint32_t bytePerPixel = m_dataFormat == GL_RGBA ? 4 : 3;
        if(size != (m_width * m_height * bytePerPixel)){
            ENGINE_LOG_ERROR("Data must be entire texture!!");
        }
        glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
    }

}