#pragma once

#include "Camera.hpp"
#include "OrthographicCamera.hpp"
#include "Texture.hpp"
#include "SubTexture2D.hpp"
#include "EditorCamera.hpp"
#include <CreepyEngine/Scene/Components.hpp>

namespace Creepy {

    class Renderer2D
    {
        public:
            static void Init() noexcept;
            static void ShutDown() noexcept;

            // TODO: Replace view matrix
            static void BeginScene(const Camera& camera, const glm::mat4& transform) noexcept;
            static void BeginScene(const EditorCamera& camera) noexcept;
            
            static void EndScene() noexcept;
            static void Flush() noexcept;

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
            static void startBatch() noexcept;
            static void flushAndReset() noexcept;
    };
    
}