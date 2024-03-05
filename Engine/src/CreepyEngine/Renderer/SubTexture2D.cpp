#include <CreepyEngine/Renderer/SubTexture2D.hpp>


namespace Creepy {

    SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& minBound, const glm::vec2& maxBound) noexcept 
        : m_texture{texture}
    {
        m_textureCoords[0] = {minBound.x, minBound.y};
        m_textureCoords[1] = {maxBound.x, minBound.y};
        m_textureCoords[2] = {maxBound.x, maxBound.y};
        m_textureCoords[3] = {minBound.x, maxBound.y};
    }

    Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& index, const glm::vec2& cellSize, const glm::vec2& spriteSize) noexcept {

        glm::vec2 minBound{(index.x * cellSize.x) / texture->GetWidth(), (index.y * cellSize.y) / texture->GetHeight()};

        glm::vec2 maxBound{((index.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((index.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };

        return std::make_shared<SubTexture2D>(texture, minBound, maxBound);
    }

}