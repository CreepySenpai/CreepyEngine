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
        Ref<Shader> shader;
        Ref<Texture2D> whiteTexture;
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

        s_renderer2dStorage->whiteTexture = Texture2D::Create(1, 1);

        uint32_t whiteTextureData = 0xffffffff;

        s_renderer2dStorage->whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        s_renderer2dStorage->shader = Shader::Create("./assets/shaders/VertexShader.glsl", "./assets/shaders/FragmentShader.glsl");

        s_renderer2dStorage->shader->Bind();
        // We need to init default texture unit
        s_renderer2dStorage->shader->SetUniformInt1("u_texture", 0);
    }

    void Renderer2D::ShutDown() noexcept {
        delete s_renderer2dStorage;
    }

    void Renderer2D::BeginScene(const OrthographicCamera &camera) noexcept {
        s_renderer2dStorage->shader->Bind();

        s_renderer2dStorage->shader->SetUniformMat4("u_viewProjectionMatrix", camera.GetViewProjectionMatrix());

    }

    void Renderer2D::EndScene() noexcept {

    }

    void Renderer2D::DrawRect(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) noexcept {
        DrawRect({position.x, position.y, 0.0f}, size, color);
    }
    
    void Renderer2D::DrawRect(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) noexcept {
        s_renderer2dStorage->shader->Bind();
        s_renderer2dStorage->shader->SetUniformFloat4("u_color", color);
        s_renderer2dStorage->shader->SetUniformFloat1("u_tilingFactor", 1.0f);

        s_renderer2dStorage->whiteTexture->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

        transform = glm::scale(transform, {size.x, size.y, 1.0f});

        s_renderer2dStorage->shader->SetUniformMat4("u_transformMatrix", transform);

        s_renderer2dStorage->vertexArray->Bind();
        RenderCommand::DrawIndex(s_renderer2dStorage->vertexArray);
    }

    void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture) noexcept {
        DrawRect(glm::vec3(position.x, position.y, 0.0f), size, texture);
    }

    void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture) noexcept {
        s_renderer2dStorage->shader->Bind();
        
        // We set pure white color to keep texture color
        s_renderer2dStorage->shader->SetUniformFloat4("u_color", glm::vec4(1.0f));
        s_renderer2dStorage->shader->SetUniformFloat1("u_tilingFactor", 1.0f);

        texture->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

        transform = glm::scale(transform, {size.x, size.y, 1.0f});

        s_renderer2dStorage->shader->SetUniformMat4("u_transformMatrix", transform);


        s_renderer2dStorage->vertexArray->Bind();
        RenderCommand::DrawIndex(s_renderer2dStorage->vertexArray);

        // texture->UnBind();
    }

    void Renderer2D::DrawRotRect(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) noexcept {

        DrawRotRect({position.x, position.y, 0.0f}, size, rotation, color);

    }
    void Renderer2D::DrawRotRect(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) noexcept {
        s_renderer2dStorage->shader->Bind();
        s_renderer2dStorage->shader->SetUniformFloat4("u_color", color);
        s_renderer2dStorage->shader->SetUniformFloat1("u_tilingFactor", 1.0f);

        s_renderer2dStorage->whiteTexture->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        transform = glm::scale(transform, {size.x, size.y, 1.0f});

        s_renderer2dStorage->shader->SetUniformMat4("u_transformMatrix", transform);

        s_renderer2dStorage->vertexArray->Bind();
        RenderCommand::DrawIndex(s_renderer2dStorage->vertexArray);
    }
 
    void Renderer2D::DrawRotRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture) noexcept {

        DrawRotRect(glm::vec3(position.x, position.y, 0.0f), size, rotation, texture);

    }
    void Renderer2D::DrawRotRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture) noexcept {
        s_renderer2dStorage->shader->Bind();
        
        // We set pure white color to keep texture color
        s_renderer2dStorage->shader->SetUniformFloat4("u_color", glm::vec4(1.0f));
        s_renderer2dStorage->shader->SetUniformFloat1("u_tilingFactor", 1.0f);

        texture->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        transform = glm::scale(transform, {size.x, size.y, 1.0f});

        s_renderer2dStorage->shader->SetUniformMat4("u_transformMatrix", transform);


        s_renderer2dStorage->vertexArray->Bind();
        RenderCommand::DrawIndex(s_renderer2dStorage->vertexArray);
    }

}