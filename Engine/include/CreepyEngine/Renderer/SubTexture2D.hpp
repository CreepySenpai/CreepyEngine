#pragma once

#include <span>
#include <CreepyEngine/Core/Core.hpp>
#include <glm/glm.hpp>
#include "Texture.hpp"

namespace Creepy
{
    class SubTexture2D
    {
        public:
            SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& minBound, const glm::vec2& maxBound) noexcept;
            
            [[nodiscard]] constexpr inline const auto& GetTexture() const noexcept {
                return m_texture;
            }

            [[nodiscard]] constexpr inline const auto& GetTextureCoords() const noexcept {
                return m_textureCoords;
            }

            static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& index, const glm::vec2& cellSize, const glm::vec2& spriteSize = glm::vec2(1.0f, 1.0f)) noexcept;
            
        private:
            Ref<Texture2D> m_texture;

            std::array<glm::vec2, 4> m_textureCoords;
    };
   
}
