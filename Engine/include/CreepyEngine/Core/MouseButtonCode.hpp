#pragma once

#include <cstdint>

namespace Creepy {

    enum class MouseButtonCode : uint32_t {
        MOUSE_BUTTON_1         = 0,
        MOUSE_BUTTON_2         = 1,
        MOUSE_BUTTON_3         = 2,
        MOUSE_BUTTON_4         = 3,
        MOUSE_BUTTON_5         = 4,
        MOUSE_BUTTON_6         = 5,
        MOUSE_BUTTON_7         = 6,
        MOUSE_BUTTON_8         = 7,
        MOUSE_LEFT             = MOUSE_BUTTON_1,
        MOUSE_RIGHT            = MOUSE_BUTTON_2,
        MOUSE_MIDDLE           = MOUSE_BUTTON_3,
        MOUSE_LAST             = MOUSE_BUTTON_8,
    };

}