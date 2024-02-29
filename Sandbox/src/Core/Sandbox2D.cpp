#include <Core/Sandbox2D.hpp>

Sandbox2D::Sandbox2D() noexcept : Creepy::Layer{"SandBox"}, m_cameraController{600.0f / 600.0f, true}{

}

void Sandbox2D::OnAttach() noexcept
{
    m_vertexArray = Creepy::VertexArray::Create();

    float vertex[] {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f, 
                0.5f, 0.5f, 0.0f,  
                -0.5f, 0.5f, 0.0f, 
    };

    m_vertexBuffer = Creepy::VertexBuffer::Create(vertex, sizeof(vertex));

    Creepy::BufferLayout vertexBufferLayout{
        {Creepy::ShaderDataType::Float3, "a_position"},
    };

    m_vertexBuffer->SetLayout(vertexBufferLayout);

    m_vertexArray->AddVertexBuffer(m_vertexBuffer); // We need add buffer after it add layout, if not it will empty

    uint32_t index[]{
        0, 1, 2, 2, 3, 0};

    m_indexBuffer = Creepy::IndexBuffer::Create(index, sizeof(index));

    m_vertexArray->SetIndexBuffer(m_indexBuffer);

    std::string vertexSources = R"(#version 460 core

            layout(location=0) in vec3 a_position;

            uniform mat4 u_viewProjectionMatrix;
            uniform mat4 u_transformMatrix;

            void main(){
                gl_Position = u_viewProjectionMatrix * u_transformMatrix * vec4(a_position, 1.0);
            }
            )";

    std::string fragmentSources = R"(#version 460 core
            uniform vec4 u_color;

            out vec4 color;

            void main(){
                color = u_color;
            })";

    m_shader = Creepy::Shader::Create("myShader", vertexSources, fragmentSources);

}

void Sandbox2D::OnDetach() noexcept
{
    
}

void Sandbox2D::OnUpdate(const Creepy::TimeStep &timeStep) noexcept
{
    m_cameraController.OnUpdate(timeStep);

    Creepy::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    Creepy::RenderCommand::Clear();

    Creepy::Renderer::BeginScene(m_cameraController.GetCamera());

    m_shader->Bind();

    std::dynamic_pointer_cast<Creepy::OpenGLShader>(m_shader)->SetUniformFloat4("u_color", m_playerColor);
    Creepy::Renderer::Submit(m_shader, m_vertexArray,glm::mat4(1.0f));

    Creepy::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender() noexcept
{
    ImGui::Begin("Color Picker");
    ImGui::ColorEdit4("My Color", glm::value_ptr(m_playerColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Creepy::Event &event) noexcept
{
    m_cameraController.OnEvent(event);
}