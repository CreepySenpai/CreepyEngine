#pragma once

#include <string>

namespace Creepy {

    class FileDialogs{
        public:
            
            // If cancelled we return empty string
            static std::string OpenFile(const char* filter) noexcept;
            static std::string SaveFile(const char* filter) noexcept;
    };

}