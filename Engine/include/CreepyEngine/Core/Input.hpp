#pragma once

#include <memory>
#include "KeyCode.hpp"
#include "MouseButtonCode.hpp"

namespace Creepy
{
    class Input
    {
        public:
            
            static bool IsKeyPressed(KeyCode keyCode) noexcept;

            static bool IsMouseButtonPressed(MouseButtonCode mouseButton) noexcept;

            static float GetMouseX() noexcept;

            static float GetMouseY() noexcept;

            static std::pair<float, float> GetMousePosition() noexcept;
    };
    
}