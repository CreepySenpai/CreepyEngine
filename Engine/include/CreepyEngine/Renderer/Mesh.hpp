#pragma once

#include <CreepyEngine/Core/Core.hpp>
#include <vector>
#include <span>
#include <glm/glm.hpp>

namespace Creepy{
    
    struct Vertex{
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec3 Normal;
        glm::vec2 TextureCoord;
    };

    struct MeshInfo{
        uint32_t Vertices;
        uint32_t Indices;
    };

    class Mesh
    {
        public:
            constexpr Mesh() noexcept = default;
            constexpr Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) noexcept 
                : m_vertices{vertices}, m_indices{indices} {}

            constexpr std::span<const Vertex> GetVertices() const noexcept {
                return m_vertices;
            }

            constexpr std::span<const uint32_t> GetIndices() const noexcept {
                return m_indices;
            }

            constexpr size_t GetTotalVertices() const noexcept {
                return m_vertices.size();
            }

            constexpr size_t GetTotalIndices() const noexcept {
                return m_indices.size();
            }

        private:
            std::vector<Vertex> m_vertices;
            std::vector<uint32_t> m_indices;
    };
}