#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Renderer/Renderer2D.hpp>
#include <CreepyEngine/Renderer/VertexArray.hpp>
#include <CreepyEngine/Renderer/Shader.hpp>
#include <CreepyEngine/Renderer/RenderCommand.hpp>

#include <Platform/OpenGL/OpenGLShader.hpp>
#include <glm/glm.hpp>

namespace Creepy {

    struct Renderer2DStorage {
        Ref<VertexArray> vertexArray;
        Ref<Shader> nonTextureShader;
        Ref<Shader> textureShader;
    };

    static Renderer2DStorage* s_renderer2dStorage;


    void Renderer2D::Init() noexcept {
        s_renderer2dStorage = new Renderer2DStorage;

        s_renderer2dStorage->vertexArray = Creepy::VertexArray::Create();

        float vertex[] {
            -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
            0.5f, -0.5f, 0.0f,      1.0f, 0.0f,
            0.5f, 0.5f, 0.0f,       1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f,      0.0f, 1.0f,
        };

        auto vertexBuffer = Creepy::VertexBuffer::Create(vertex, sizeof(vertex));

        Creepy::BufferLayout vertexBufferLayout{
            {Creepy::ShaderDataType::Float3, "a_position"},
            {Creepy::ShaderDataType::Float2, "a_textureCoord"},
        };

        vertexBuffer->SetLayout(vertexBufferLayout);

        s_renderer2dStorage->vertexArray->AddVertexBuffer(vertexBuffer); // We need add buffer after it add layout, if not it will empty

        uint32_t index[]{
            0, 1, 2, 2, 3, 0
        };

        auto indexBuffer = Creepy::IndexBuffer::Create(index, sizeof(index));

        s_renderer2dStorage->vertexArray->SetIndexBuffer(indexBuffer);

        s_renderer2dStorage->nonTextureShader = Shader::Create("./assets/shaders/VertexShader.glsl", "./assets/shaders/FragmentShader.glsl");

        s_renderer2dStorage->textureShader = Shader::Create("./assets/shaders/VertexShaderWithTexture.glsl", "./assets/shaders/FragmentShaderWithTexture.glsl");

        s_renderer2dStorage->textureShader->Bind();
        // We need to init default texture unit
        s_renderer2dStorage->textureShader->SetUniformInt1("u_texture", 0);
    }

    void Renderer2D::ShutDown() noexcept {
        delete s_renderer2dStorage;
    }

    void Renderer2D::BeginScene(const OrthographicCamera &camera) noexcept {
        
        s_renderer2dStorage->nonTextureShader->Bind();
        
        s_renderer2dStorage->nonTextureShader->SetUniformMat4("u_viewProjectionMatrix", camera.GetViewProjectionMatrix());

        s_renderer2dStorage->textureShader->Bind();

        s_renderer2dStorage->textureShader->SetUniformMat4("u_viewProjectionMatrix", camera.GetViewProjectionMatrix());

    }

    void Renderer2D::EndScene() noexcept {

    }

    void Renderer2D::DrawRect(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) noexcept {
        DrawRect({position.x, position.y, 0.0f}, size, color);
    }
    
    void Renderer2D::DrawRect(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) noexcept {
        s_renderer2dStorage->nonTextureShader->Bind();
        s_renderer2dStorage->nonTextureShader->SetUniformFloat4("u_color", color);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

        // TODO: Add rotation here

        transform = glm::scale(transform, {size.x, size.y, 1.0f});

        s_renderer2dStorage->nonTextureShader->SetUniformMat4("u_transformMatrix", transform);

        s_renderer2dStorage->vertexArray->Bind();
        RenderCommand::DrawIndex(s_renderer2dStorage->vertexArray);
    }

    void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture) noexcept {
        DrawRect(glm::vec3(position.x, position.y, 0.0f), size, texture);
    }

    void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture) noexcept {
        s_renderer2dStorage->textureShader->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

        // TODO: Add rotation here

        transform = glm::scale(transform, {size.x, size.y, 1.0f});

        s_renderer2dStorage->textureShader->SetUniformMat4("u_transformMatrix", transform);

        texture->Bind();

        s_renderer2dStorage->vertexArray->Bind();
        RenderCommand::DrawIndex(s_renderer2dStorage->vertexArray);
    }

}