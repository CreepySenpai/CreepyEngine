#include <CreepyEngine/Renderer/Mesh.hpp>
#include <CreepyEngine/Renderer/VertexArray.hpp>


namespace Creepy {

    void Mesh::setUpMesh() noexcept {
        m_vertexArray = VertexArray::Create();

        m_vertexArray->Bind();
        // m_vertexArray->GetVertexBuffers()
    }

}