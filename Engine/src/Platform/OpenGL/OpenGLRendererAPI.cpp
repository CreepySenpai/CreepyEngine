#include <Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <CreepyEngine/Renderer/VertexArray.hpp>
#include <glad/glad.h>

namespace Creepy {

    void OpenGLRendererAPI::Init() noexcept {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LINE_SMOOTH);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color) noexcept
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear() noexcept
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndex(const Ref<VertexArray>& vertexArray, uint32_t indexCount) noexcept {
        // For sure we render match vertex array
        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) noexcept {
        
        vertexArray->Bind();
        glDrawArrays(GL_LINES, 0, vertexCount);
    }

    void OpenGLRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetLineThickness(float thickness) noexcept {
        glLineWidth(thickness);
    }

    void OpenGLRendererAPI::EnableDepth() noexcept {
        glEnable(GL_DEPTH_TEST);
    }
    
    void OpenGLRendererAPI::DisableDepth() noexcept {
        glDisable(GL_DEPTH_TEST);
    }
}