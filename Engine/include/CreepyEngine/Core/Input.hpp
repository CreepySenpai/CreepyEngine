#pragma once

#include <memory>

namespace Creepy
{
    class Input
    {
        public:
            
            static bool IsKeyPressed(int keyCode) noexcept;

            static bool IsMouseButtonPressed(int mouseButton) noexcept;

            static float GetMouseX() noexcept;

            static float GetMouseY() noexcept;

            static std::pair<float, float> GetMousePosition() noexcept;
    };
    
}