#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Creepy {

    struct TagComponent{
        std::string Tag{""};

        constexpr TagComponent() noexcept = default;
        
        constexpr TagComponent(const std::string& tag) noexcept : Tag{tag} {}
    };

    struct TransformComponent
    {
        glm::mat4 Transform{1.0f};

        constexpr TransformComponent() noexcept = default;

        constexpr TransformComponent(const glm::mat4& transform) noexcept : Transform{transform} {}

    };

    struct SpriteComponent {

        glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};

        constexpr SpriteComponent() noexcept = default;

        constexpr SpriteComponent(const glm::vec4& color) noexcept : Color{color} {}

    };
    

}