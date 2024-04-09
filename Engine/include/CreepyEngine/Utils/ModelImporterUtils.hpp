#pragma once

#include <filesystem>
#include <CreepyEngine/Core/Core.hpp>
namespace Creepy {
    class Mesh;
    class Model;
}
namespace Creepy::Utils {

    class ModelImporter {
        
        public:
            static std::vector<Ref<Mesh>> LoadMesh(const std::filesystem::path& filePath) noexcept;
            static Model LoadModel(const std::filesystem::path& filePath) noexcept;
        private:

    };

}