#pragma once

#include <filesystem>
#include <cstdint>
#include "Mesh.hpp"

namespace Creepy {

    class VertexArray;

    class Model {

        public:

            constexpr Model() noexcept = default;
            
            Model(const std::vector<Mesh>& meshes) noexcept;

            constexpr const std::vector<MeshInfo>& GetMeshesInfo() const noexcept {
                return m_meshesInfo;
            }

            Mesh& GetMeshAt(uint32_t index) noexcept;

            uint32_t TotalMeshes() const noexcept;

            uint32_t TotalVertices() const noexcept;

            uint32_t TotalIndices() const noexcept;

            static Model Create(const std::filesystem::path& filePath) noexcept;

        private:
            std::vector<MeshInfo> m_meshesInfo;
            std::vector<Mesh> m_meshes;
    };

}