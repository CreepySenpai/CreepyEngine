#pragma once

#include "OrthographicCamera.hpp"

namespace Creepy {

    class Renderer2D
    {
        public:
            static void Init() noexcept;
            static void ShutDown() noexcept;

            static void BeginScene(const OrthographicCamera& camera) noexcept;
            static void EndScene() noexcept;

            static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) noexcept;
            static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) noexcept;
    };
    
}