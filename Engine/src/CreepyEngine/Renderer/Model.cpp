#include <CreepyEngine/Renderer/Model.hpp>
#include <CreepyEngine/Utils/ModelImporterUtils.hpp>

namespace Creepy {
    
    Model Model::Create(const std::filesystem::path& filePath) noexcept {
        return Utils::ModelImporter::LoadModel(filePath);
    }

    Model::Model(const std::vector<Mesh>& meshes) noexcept : m_meshes{meshes} {

        for(auto& mesh : meshes){
            m_meshesInfo.emplace_back(mesh.GetTotalVertices(), mesh.GetTotalIndices());
        }

    }

    Mesh& Model::GetMeshAt(uint32_t index) noexcept {
        return m_meshes.at(index);
    }

    uint32_t Model::TotalMeshes() const noexcept {
        return m_meshes.size();
    }

    // TODO: Use std::reduce
    uint32_t Model::TotalVertices() const noexcept {
        uint32_t total{};

        for(auto& info : m_meshesInfo) {
            total += info.Vertices;
        }

        return total;
    }

    uint32_t Model::TotalIndices() const noexcept {
        uint32_t total{};
        for(auto& info : m_meshesInfo) {
            total += info.Indices;
        }

        return total;
    }
    
}