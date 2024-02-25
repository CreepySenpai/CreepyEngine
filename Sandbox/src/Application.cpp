#include <iostream>
#include <CreepyEngine.hpp>

class GameLayer : public Creepy::Layer
{
    public:

        GameLayer() noexcept : Creepy::Layer{"Game Layer"}, m_camera{-1.0f, 1.0f, -1.0f, 1.0f} {
            
            m_vertexArray.reset(Creepy::VertexArray::Create());
        
            float vertex[] {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
                0.5f, 0.5f, 0.0f,   1.0f, 1.0f,
                -0.5f, 0.5f, 0.0f,  0.0f, 1.0f,
            };

            m_vertexBuffer.reset(Creepy::VertexBuffer::Create(vertex, sizeof(vertex)));
            
            Creepy::BufferLayout vertexBufferLayout{
                {Creepy::ShaderDataType::Float3, "a_position"},
                {Creepy::ShaderDataType::Float2, "a_textureCoord"}
            };

            m_vertexBuffer->SetLayout(vertexBufferLayout);

            m_vertexArray->AddVertexBuffer(m_vertexBuffer); // We need add buffer after it add layout, if not it will empty

            uint32_t index[] {
                0, 1, 2, 2, 3, 0
            };

            m_indexBuffer.reset(Creepy::IndexBuffer::Create(index, sizeof(index)));
            
            m_vertexArray->SetIndexBuffer(m_indexBuffer);

            std::string vertexSources {R"-(#version 460 core
            
            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec2 a_textureCoord;
            
            uniform mat4 u_viewProjectionMatrix;
            uniform mat4 u_transformMatrix;

            out vec2 textureCoord;

            void main(){
                textureCoord = a_textureCoord;
                gl_Position = u_viewProjectionMatrix * u_transformMatrix * vec4(a_position, 1.0);
            }
            
            )-"};

            std::string fragmentSources {R"-(#version 460 core
            
            uniform vec4 u_color;
            uniform sampler2D u_texture;

            out vec4 color;
            in vec2 textureCoord;
            
            void main(){
                vec4 c = u_color;
                color = texture(u_texture, textureCoord);
            }

            )-"};

            m_shader.reset(Creepy::Shader::Create(vertexSources, fragmentSources));

            m_texture = Creepy::Texture2D::Create("./assets/textures/SpecularMap.png");
            m_texture->Bind(0);
            
            std::dynamic_pointer_cast<Creepy::OpenGLShader>(m_shader)->SetUniformInt1("u_texture", 0);
        }

        constexpr virtual void OnAttach() noexcept override {

        }
        constexpr virtual void OnDetach() noexcept override {

        }

        constexpr virtual void OnUpdate(const Creepy::TimeStep& timeStep) noexcept override {
            float moveSpeed{0.1f};

            APP_LOG_INFO("Delta Time: {}, {}", timeStep.GetSeconds(), timeStep.GetMilliseconds());

            if(Creepy::Input::IsKeyPressed(CREEPY_KEY_RIGHT)){
                m_cameraPosition.x += moveSpeed * timeStep.GetSeconds();
            }
            else if(Creepy::Input::IsKeyPressed(CREEPY_KEY_LEFT)){
                m_cameraPosition.x -= moveSpeed * timeStep.GetSeconds();
            }
            else if(Creepy::Input::IsKeyPressed(CREEPY_KEY_UP)){
                m_cameraPosition.y += moveSpeed * timeStep.GetSeconds();
            }
            else if(Creepy::Input::IsKeyPressed(CREEPY_KEY_DOWN)){
                m_cameraPosition.y -= moveSpeed * timeStep.GetSeconds();
            }

            if(Creepy::Input::IsKeyPressed(CREEPY_KEY_W)){
                m_playerPosition.y += moveSpeed * timeStep.GetSeconds();
            }
            else if(Creepy::Input::IsKeyPressed(CREEPY_KEY_S)){
                m_playerPosition.y -= moveSpeed * timeStep.GetSeconds();
            }
            else if(Creepy::Input::IsKeyPressed(CREEPY_KEY_A)){
                m_playerPosition.x -= moveSpeed * timeStep.GetSeconds();
            }
            else if(Creepy::Input::IsKeyPressed(CREEPY_KEY_D)){
                m_playerPosition.x += moveSpeed * timeStep.GetSeconds();
            }

            m_camera.SetPosition(m_cameraPosition);

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_playerPosition);

            Creepy::RenderCommand::Clear();
            
            Creepy::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});

            Creepy::Renderer::BeginScene(m_camera);
            
            m_shader->Bind();

            std::dynamic_pointer_cast<Creepy::OpenGLShader>(m_shader)->SetUniformFloat4("u_color", m_playerColor);

           

            Creepy::Renderer::Submit(m_shader, m_vertexArray, transform);

            Creepy::Renderer::EndScene();
        }

        constexpr virtual void OnImGuiRender() noexcept override {
            ImGui::Begin("Color Picker");
            ImGui::ColorEdit4("My Color", glm::value_ptr(m_playerColor));
            ImGui::End();
        }

        constexpr virtual void OnEvent(Creepy::Event &event) noexcept override {

            Creepy::EventDispatcher dispatcher{event};

            dispatcher.Dispatch<Creepy::KeyPressedEvent>(std::bind_front(GameLayer::OnKeyPressed, this));

        }

        constexpr bool OnKeyPressed(Creepy::KeyPressedEvent& event) noexcept {

            return false;
        }

    private:
        Creepy::Ref<Creepy::Shader> m_shader;
        Creepy::Ref<Creepy::VertexArray> m_vertexArray;
        Creepy::Ref<Creepy::VertexBuffer> m_vertexBuffer;
        Creepy::Ref<Creepy::IndexBuffer> m_indexBuffer;
        Creepy::Ref<Creepy::Texture2D> m_texture;

        Creepy::OrthographicCamera m_camera;

        glm::vec3 m_cameraPosition{0.0f, 0.0f, 0.0f};
        glm::vec3 m_playerPosition{0.0f, 0.0f, 0.0f};
        glm::vec4 m_playerColor{1.0f, 0.0f, 0.0f, 1.0f};
};

class SandboxApplication : public Creepy::Application {
    public:
        SandboxApplication() noexcept{
            std::clog << "Wow\n";
            this->PushLayer(new GameLayer());
        }

        virtual ~SandboxApplication() noexcept {
            
        }
    private:
};

std::unique_ptr<Creepy::Application> Creepy::CreateApplication() {
    return std::make_unique<SandboxApplication>();
}