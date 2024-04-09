#pragma once

#include <span>
#include <CreepyEngine/Core/Core.hpp>
#include "RendererAPI.hpp"
#include <glm/glm.hpp>

namespace Creepy {

    //Forward Declare

    class Camera;
    class EditorCamera;
    class Texture2D;
    class SubTexture2D;

    struct TransformComponent;
    struct SpriteComponent;
    struct CircleSpriteComponent;

    class Renderer
    {
        public:

            static constexpr inline RendererAPI::API GetRenderAPI() noexcept {
                return RendererAPI::GetAPI();
            }

            static void WindowResize(uint32_t width, uint32_t height) noexcept;

            static void Init() noexcept;
            static void ShutDown() noexcept;

            // TODO: Replace view matrix
            static void BeginScene(const Camera& camera, const glm::mat4& transform) noexcept;
            static void BeginScene(const EditorCamera& camera) noexcept;
            
            static void EndScene() noexcept;
            static void Flush2DBatch() noexcept;
            static void Flush3DBatch() noexcept;

            static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) noexcept;
            static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) noexcept;

            static void DrawRect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tilingColor = glm::vec4(1.0f)) noexcept;
            static void DrawRect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tilingColor = glm::vec4(1.0f)) noexcept;

            static void DrawRect(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tilingColor = glm::vec4(1.0f)) noexcept;
            static void DrawRect(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tilingColor = glm::vec4(1.0f)) noexcept;

            static void DrawRect(const glm::mat4& transform, const glm::vec4& color, float tilingFactor = 1.0f, int entityID = -1) noexcept;
            static void DrawRect(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tilingColor = glm::vec4(1.0f), float tilingFactor = 1.0f, int entityID = -1) noexcept;

            static void DrawSprite(TransformComponent& transform, SpriteComponent& sprite, int entityID = -1) noexcept;

            static void DrawCircle(TransformComponent& transform, CircleSpriteComponent& circle, int entityID = -1) noexcept;

            static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickNess = 1.0f, float fade = 0.005f, int entityID = -1) noexcept;

            static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int entityID = -1) noexcept;

            static void DrawLineRect(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, int entityID = -1) noexcept;

            static void DrawLineRect(const glm::mat4& transform, const glm::vec4& color, int entityID = -1) noexcept;

            // 3D

            static void DrawCube(const glm::mat4& transform, const glm::vec4& color, int entityID = -1) noexcept;

            struct Statistics{
                uint32_t DrawCalls{0};
                uint32_t RectCount{0};

                constexpr inline uint32_t GetTotalVertexCount() const noexcept {
                    return RectCount * 4;
                }

                constexpr inline uint32_t GetTotalIndexCount() const noexcept {
                    return RectCount * 6;
                }
            };

            static void ResetStatistics() noexcept;
            static Statistics GetStatistics() noexcept;

        private:
            static void setRectProperty(const glm::mat4& transform, const glm::vec4& color, const std::array<glm::vec2, 4>& textureCoords, float textureIndex, float tilingFactor = 1.0f, int entityID = -1) noexcept;

            // TODO: Add Texture Coord When Use SubTexture
            // static void setCubeProperty(const glm::mat4& transform, const glm::vec4& color, std::span<glm::vec3> textureCoords, float textureIndex, int entityID = -1) noexcept;
            static void setCubeProperty(const glm::mat4& transform, const glm::vec4& color, float textureIndex, int entityID = -1) noexcept;

            static void start2DBatch() noexcept;
            static void start3DBatch() noexcept;
            static void flushAndReset2DBatch() noexcept;
            static void flushAndReset3DBatch() noexcept;
            static void check2DBatchNeedReset() noexcept;
            static void check3DBatchNeedReset() noexcept;
            static void checkTextureNeedReset(float& textureIndex, const Ref<Texture2D>& texture) noexcept;
    };
    
}