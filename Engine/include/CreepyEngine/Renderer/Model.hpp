#pragma once

#include <filesystem>
#include <cstdint>
#include <CreepyEngine/Core/Core.hpp>
#include "Mesh.hpp"

namespace Creepy {

    class VertexArray;

    class Model {

        public:

            constexpr Model() noexcept = default;
            
            Model(const std::vector<Ref<Mesh>>& meshes) noexcept;

            Mesh& GetMeshAt(uint32_t index) noexcept;

            uint32_t TotalMeshes() const noexcept;

            uint32_t TotalVertices() const noexcept;

            uint32_t TotalIndices() const noexcept;

            void ReleaseMesh() noexcept;

            static Model Create(const std::filesystem::path& filePath) noexcept;

        private:
            std::vector<Ref<Mesh>> m_meshes;
            uint32_t m_totalVertices{};
            uint32_t m_totalIndices{};
    };

}