#pragma once

#include <filesystem>
#include <CreepyEngine/Renderer/Model.hpp>

namespace Creepy::Utils {

    class ModelImporter {
        
        public:
            static Model LoadModel(const std::filesystem::path& filePath) noexcept;
        private:

    };

}