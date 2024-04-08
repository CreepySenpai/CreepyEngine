#pragma once

#include <CreepyEngine/Core/Core.hpp>
#include <vector>
#include <glm/glm.hpp>

namespace Creepy{
    
    struct Vertex{
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TextureCoord;
    };

    class VertexArray;

    class Mesh
    {
        public:
            constexpr Mesh() noexcept = default;
            constexpr Mesh(const std::vector<Vertex>& vertex, const std::vector<uint32_t>& indices) noexcept 
                : m_vertex{vertex}, m_indices{indices} {}

        private:

            void setUpMesh() noexcept;
        private:
            std::vector<Vertex> m_vertex;
            std::vector<uint32_t> m_indices;

            Ref<VertexArray> m_vertexArray;
    };
}