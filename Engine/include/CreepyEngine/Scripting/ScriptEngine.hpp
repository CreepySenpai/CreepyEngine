#pragma once

#include <string_view>

namespace Creepy{

    class ScriptEngine
    {
        public:
            static void Init() noexcept;

            static void ShutDown() noexcept;

        private:
            static void initCoral() noexcept;

            static void shutDownCoral() noexcept;
    };
}