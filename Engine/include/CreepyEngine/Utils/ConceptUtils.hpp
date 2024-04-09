#pragma once

#include <concepts>

namespace Creepy::Utils {

    template<typename T, typename ... U>
    concept IsAnyOf = (std::same_as<T, U> || ...);

}