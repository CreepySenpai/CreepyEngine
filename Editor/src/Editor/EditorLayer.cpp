#include <Editor/EditorLayer.hpp>


namespace Creepy {
    static char buffer[256];

    EditorLayer::EditorLayer() noexcept : Layer{"LevelEditor"}, m_cameraController{1.0f} {
        Renderer::Init();

        FrameBufferSpecification spec{.Width = 700, .Height = 700};
        m_frameBuffer = FrameBuffer::Create(spec);

        m_scene = std::make_shared<Scene>();

        m_texture = Texture2D::Create("./assets/textures/SpecularMap.png");

        m_entity = m_scene->CreateEntity();

        m_entity.AddComponent<SpriteComponent>(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});

        class Test : public ScriptableEntity{
            protected:

                void OnCreate() noexcept override {

                }

                void OnUpdate(TimeStep timeStep) noexcept override {
                    auto& position = GetComponent<TransformComponent>().Position;

                    if(Input::IsKeyPressed(KeyCode::KEY_LEFT)){
                        position.x -= 2.0f * timeStep.GetSeconds();
                    }

                    if(Input::IsKeyPressed(KeyCode::KEY_RIGHT)){
                        position.x += 2.0f * timeStep.GetSeconds();
                    }

                    if(Input::IsKeyPressed(KeyCode::KEY_UP)){
                        position.y += 2.0f * timeStep.GetSeconds();
                    }

                    if(Input::IsKeyPressed(KeyCode::KEY_DOWN)){
                        position.y -= 2.0f * timeStep.GetSeconds();
                    }

                    // APP_LOG_WARNING("Time By Script {}", timeStep.GetSeconds());

                }

                void OnDestroy() noexcept override {

                }
        };

        m_entity.AddComponent<NativeScriptComponent>().Bind<Test>();

        m_camera = m_scene->CreateEntity("Camera");
        m_camera.AddComponent<CameraComponent>().FixedAspectRatio = true;

        m_hierarchyPanel.SetScene(m_scene);
    }

    EditorLayer::~EditorLayer() noexcept {
        Renderer::ShutDown();
    }

    void EditorLayer::OnAttach() noexcept {

    }

    void EditorLayer::OnDetach() noexcept {

    }

    void EditorLayer::OnUpdate(TimeStep timeStep) noexcept {

        if(m_viewPortFocused){
            // m_cameraController.OnUpdate(timeStep);
        }


        m_frameBuffer->Bind();

        RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
        RenderCommand::Clear();

        // Renderer2D::BeginScene(m_cameraController.GetCamera());

        m_scene->OnUpdate(timeStep);

        // Renderer2D::DrawRotRect({10.0f, 4.0f, 0.0f}, {2.0f, 1.5f}, glm::radians(45.0f), {0.0f, 1.0f, 0.0f, 1.0f});
        // Renderer2D::DrawRect({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, m_texture);

        // Renderer2D::EndScene();
        
        m_frameBuffer->UnBind();
    }

    void EditorLayer::OnImGuiRender() noexcept {
        static bool p_open = false;
        static bool opt_fullscreen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;
        
        ImGui::Begin("DockSpace Demo", &p_open, window_flags);

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::Separator();

                if (ImGui::MenuItem("Close", NULL, false)){
                    Creepy::Application::GetInstance().Close();
                }
                    
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.0f, 0.0f});
        ImGui::Begin("ViewPort");

        m_viewPortFocused = ImGui::IsWindowFocused();
        m_viewPortHovered = ImGui::IsWindowHovered();

        Application::GetInstance().GetImGuiLayer().BlockEvents(!m_viewPortFocused || !m_viewPortHovered);

        auto viewPortSize = ImGui::GetContentRegionAvail();

        // Check if view port change we resize it
        if((m_viewPortSize.x != viewPortSize.x) || (m_viewPortSize.y != viewPortSize.y)) {
            m_viewPortSize.x = viewPortSize.x;
            m_viewPortSize.y = viewPortSize.y;

            m_frameBuffer->Resize(static_cast<uint32_t>(m_viewPortSize.x), static_cast<uint32_t>(m_viewPortSize.y));

            m_scene->OnViewPortResize(m_viewPortSize.x, m_viewPortSize.y);
            // m_cameraController.OnResize(m_viewPortSize.x, m_viewPortSize.y);
        }
        

        // auto id = m_texture->GetRendererID();
        auto id = m_frameBuffer->GetColorAttachmentID();
        ImGui::Image((void*)id, ImVec2{viewPortSize.x, viewPortSize.y}, ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});
        ImGui::End();
        ImGui::PopStyleVar();

        m_hierarchyPanel.OnImGuiRender();

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event &event) noexcept {
        if(m_viewPortFocused){

            // m_cameraController.OnEvent(event);

        }
    }
}