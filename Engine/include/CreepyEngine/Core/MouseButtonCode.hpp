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

    };

    enum class MouseCode : uint32_t {

        MOUSE_LAST      = 7,
        MOUSE_LEFT      = 0,
        MOUSE_RIGHT     = 1,
        MOUSE_MIDDLE    = 2,

    };

}