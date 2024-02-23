#include <iostream>
#include <CreepyEngine.hpp>

class GameLayer : public Creepy::Layer
{
    public:

        GameLayer() noexcept : Creepy::Layer{"Game Layer"}, m_camera{-1.0f, 1.0f, -1.0f, 1.0f} {
            
            m_vertexArray.reset(Creepy::VertexArray::Create());
        
            float vertex[] {
                -0.5f, -0.5f, 0.0f,
                0.0f, 0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
            };

            m_vertexBuffer.reset(Creepy::VertexBuffer::Create(vertex, sizeof(vertex)));
            
            Creepy::BufferLayout vertexBufferLayout{
                {Creepy::ShaderDataType::Float3, "a_position"}
            };

            m_vertexBuffer->SetLayout(vertexBufferLayout);

            m_vertexArray->AddVertexBuffer(m_vertexBuffer); // We need add buffer after it add layout, if not it will empty

            uint32_t index[] {
                0, 1, 2,
            };

            m_indexBuffer.reset(Creepy::IndexBuffer::Create(index, 3));
            
            m_vertexArray->SetIndexBuffer(m_indexBuffer);

            std::string vertexSources {R"-(#version 460 core
            
            layout(location = 0) in vec3 a_position;

            uniform mat4 u_viewProjectionMatrix;

            void main(){
                gl_Position = u_viewProjectionMatrix * vec4(a_position, 1.0);
            }
            
            )-"};

            std::string fragmentSources {R"-(#version 460 core
            
            out vec4 color;
            

            void main(){
                color = vec4(1.0, 0.1, 0.1, 1.0);
            }

            )-"};

            m_shader.reset(new Creepy::Shader(vertexSources, fragmentSources));

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

            m_camera.SetPosition(m_cameraPosition);

            Creepy::RenderCommand::Clear();
            
            Creepy::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});

            Creepy::Renderer::BeginScene(m_camera);

            Creepy::Renderer::Submit(m_shader, m_vertexArray);

            Creepy::Renderer::EndScene();
        }
        constexpr virtual void OnImGuiRender() noexcept override {

        }

        constexpr virtual void OnEvent(Creepy::Event &event) noexcept override {

            Creepy::EventDispatcher dispatcher{event};

            dispatcher.Dispatch<Creepy::KeyPressedEvent>(std::bind_front(GameLayer::OnKeyPressed, this));

        }

        constexpr bool OnKeyPressed(Creepy::KeyPressedEvent& event) noexcept {

            return false;
        }

    private:
        std::shared_ptr<Creepy::Shader> m_shader;
        std::shared_ptr<Creepy::VertexArray> m_vertexArray;
        std::shared_ptr<Creepy::VertexBuffer> m_vertexBuffer;
        std::shared_ptr<Creepy::IndexBuffer> m_indexBuffer;

        Creepy::OrthographicCamera m_camera;

        glm::vec3 m_cameraPosition{0.0f, 0.0f, 0.0f};
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