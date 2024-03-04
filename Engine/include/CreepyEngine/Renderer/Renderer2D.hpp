#pragma once

#include "OrthographicCamera.hpp"
#include "Texture.hpp"

namespace Creepy {

    class Renderer2D
    {
        public:
            static void Init() noexcept;
            static void ShutDown() noexcept;

            static void BeginScene(const OrthographicCamera& camera) noexcept;
            static void EndScene() noexcept;
            static void Flush() noexcept;

            static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) noexcept;
            static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) noexcept;

            static void DrawRect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tilingColor = glm::vec4(1.0f)) noexcept;
            static void DrawRect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tilingColor = glm::vec4(1.0f)) noexcept;


            static void DrawRotRect(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) noexcept;
            static void DrawRotRect(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) noexcept;
 
            static void DrawRotRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tilingColor = glm::vec4(1.0f)) noexcept;
            static void DrawRotRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tilingColor = glm::vec4(1.0f)) noexcept;


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
            static void flushAndReset() noexcept;
    };
    
}