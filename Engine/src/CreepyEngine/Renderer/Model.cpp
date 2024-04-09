#include <numeric>
#include <CreepyEngine/Renderer/Model.hpp>
#include <CreepyEngine/Utils/ModelImporterUtils.hpp>

namespace Creepy {
    
    Model Model::Create(const std::filesystem::path& filePath) noexcept {
        return Utils::ModelImporter::LoadModel(filePath);
    }

    Model::Model(const std::vector<Ref<Mesh>>& meshes) noexcept : m_meshes{meshes} {

        for(auto& mesh : m_meshes) {
            m_totalVertices += mesh->GetTotalVertices();
            m_totalIndices += mesh->GetTotalIndices();
        }

    }

    Mesh& Model::GetMeshAt(uint32_t index) noexcept {
        return *m_meshes.at(index);
    }

    uint32_t Model::TotalMeshes() const noexcept {
        return m_meshes.size();
    }

    uint32_t Model::TotalVertices() const noexcept {
        return m_totalVertices;
    }

    uint32_t Model::TotalIndices() const noexcept {
        return m_totalIndices;
    }

    void Model::ReleaseMesh() noexcept {
        m_meshes.clear();
    }
    
}