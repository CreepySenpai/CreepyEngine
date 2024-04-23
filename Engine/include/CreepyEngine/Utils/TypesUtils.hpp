#pragma once

#include <cstddef>

namespace Creepy{

    template <typename... Ts>
    using AlignMentBuffer = alignas(Ts...) std::byte[std::max({sizeof(Ts)...})];

}