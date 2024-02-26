#include <Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <glad/glad.h>

namespace Creepy {

    void OpenGLRendererAPI::Init() noexcept {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color) noexcept
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear() noexcept
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndex(const Ref<VertexArray> &vertexArray) noexcept
    {
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        
    }
}