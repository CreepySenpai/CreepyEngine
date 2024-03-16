#include <Editor/EditorLayer.hpp>

namespace Creepy {
    static char buffer[256];

    EditorLayer::EditorLayer() noexcept : Layer{"LevelEditor"}, m_cameraController{1.0f} {
        Renderer::Init();

        FrameBufferSpecification spec{.Width = 700, .Height = 700};
        m_frameBuffer = FrameBuffer::Create(spec);

        m_scene = std::make_shared<Scene>();

        m_texture = Texture2D::Create("./assets/textures/SpecularMap.png");

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

        // if(FrameBufferSpecification spec = m_frameBuffer->Get)

        if(m_viewPortFocused){
            // m_cameraController.OnUpdate(timeStep);
        }


        m_frameBuffer->Bind();

        RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
        RenderCommand::Clear();
        Renderer2D::ResetStatistics();
        m_scene->OnUpdate(timeStep);
        
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
        
        auto& style = ImGui::GetStyle();

        // Apply to all property window
        const float minWinsize = style.WindowMinSize.x;

        // We limit window size
        style.WindowMinSize.x = 400.0f;

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinsize;
        

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::Separator();

                if(ImGui::MenuItem("New", "Ctrl+N")){
                    this->NewScene();
                }

                ImGui::Separator();

                if(ImGui::MenuItem("Open...", "Ctrl+O")){
                    this->OpenScene();
                }

                ImGui::Separator();

                if(ImGui::MenuItem("Save As...", "Ctrl+S")){
                    this->SaveSceneAs();
                }

                ImGui::Separator();

                if(ImGui::MenuItem("Save Theme")){
                    Application::GetInstance().GetImGuiLayer().SaveThemeToYaml("./assets/scenes/test.thm");
                }

                ImGui::Separator();

                if(ImGui::MenuItem("Load Theme")){
                    if(Application::GetInstance().GetImGuiLayer().LoadThemeFromYaml("./assets/scenes/test.thm")){
                        Application::GetInstance().GetImGuiLayer().SetTheme();
                    }
                }

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

        }
        

        // auto id = m_texture->GetRendererID();
        auto id = m_frameBuffer->GetColorAttachmentID();
        ImGui::Image((void*)id, ImVec2{viewPortSize.x, viewPortSize.y}, ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});
        ImGui::End();
        ImGui::PopStyleVar();

        m_hierarchyPanel.OnImGuiRender();


        ImGui::Begin("Statics");

        auto stats = Creepy::Renderer2D::GetStatistics();

        ImGui::Text("Render2D Stats");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Rect Count: %d", stats.RectCount);
        ImGui::Text("Total Vertex: %d", stats.GetTotalVertexCount());
        ImGui::Text("Total Index: %d", stats.GetTotalIndexCount());

        ImGui::End();

        {
            ImGui::Begin("Theme Setting");
            auto&& imguiInstance = Application::GetInstance().GetImGuiLayer();
            auto&& editorConfig = imguiInstance.GetEditorConfig();

            if(ImGui::ColorEdit4("Window Background", glm::value_ptr(editorConfig.WindowBg))){
                imguiInstance.SetTheme();
            }

            if(ImGui::ColorEdit4("Header", glm::value_ptr(editorConfig.Header))){
                imguiInstance.SetTheme();
            }

            if(ImGui::ColorEdit4("Header Hovered", glm::value_ptr(editorConfig.HeaderHovered))){
                imguiInstance.SetTheme();
            }

            if(ImGui::ColorEdit4("Header Active", glm::value_ptr(editorConfig.HeaderActive))){
                imguiInstance.SetTheme();
            }

            if(ImGui::ColorEdit4("Button", glm::value_ptr(editorConfig.Button))){
                imguiInstance.SetTheme();
            }

            if(ImGui::ColorEdit4("Button Hovered", glm::value_ptr(editorConfig.ButtonHovered))){
                imguiInstance.SetTheme();
            }

            if(ImGui::ColorEdit4("Button Active", glm::value_ptr(editorConfig.ButtonActive))){
                imguiInstance.SetTheme();
            }

            if(ImGui::ColorEdit4("Frame Background", glm::value_ptr(editorConfig.FrameBg))){
                imguiInstance.SetTheme();
            }

            if(ImGui::ColorEdit4("FrameBg Hovered", glm::value_ptr(editorConfig.FrameBgHovered))){
                imguiInstance.SetTheme();
            }

            if(ImGui::ColorEdit4("FrameBg Active", glm::value_ptr(editorConfig.FrameBgActive))) {
                imguiInstance.SetTheme();
            }

            if(ImGui::ColorEdit4("Tab", glm::value_ptr(editorConfig.Tab))) {
                imguiInstance.SetTheme();
            }

            if(ImGui::ColorEdit4("Tab Hovered", glm::value_ptr(editorConfig.TabHovered))) {
                imguiInstance.SetTheme();
            }
            
            if(ImGui::ColorEdit4("Tab Active", glm::value_ptr(editorConfig.TabActive))) {
                imguiInstance.SetTheme();
            }

            if(ImGui::ColorEdit4("Tab Unfocused", glm::value_ptr(editorConfig.TabUnfocused))) {
                imguiInstance.SetTheme();
            }

            if(ImGui::ColorEdit4("Tab Unfocused Active", glm::value_ptr(editorConfig.TabUnfocusedActive))) {
                imguiInstance.SetTheme();
            }


            ImGui::End();
        }

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event &event) noexcept {
        EventDispatcher dispatcher{event};

        dispatcher.Dispatch<KeyPressedEvent>(std::bind_front(OnKeyPressed, this));
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& event) noexcept {
        if(event.GetRepeatCount() > 0){
            return false;
        }

        bool ctrl = Input::IsKeyPressed(KeyCode::KEY_LEFT_CONTROL) || Input::IsKeyPressed(KeyCode::KEY_RIGHT_CONTROL);

        switch (event.GetKeyCode())
        {
            case std::to_underlying(KeyCode::KEY_N) : {
                if(ctrl){
                    this->NewScene();
                }
                break;
            }

            case std::to_underlying(KeyCode::KEY_O) : {
                if(ctrl){
                    this->OpenScene();
                }
                break;
            }

            case std::to_underlying(KeyCode::KEY_S) : {
                if(ctrl){
                    this->SaveSceneAs();
                }
                break;
            }
        }

        return false;
    }


    void EditorLayer::NewScene() noexcept {
        m_scene = std::make_shared<Scene>();    // Create New Empty Scene
        m_scene->OnViewPortResize(static_cast<uint32_t>(m_viewPortSize.x), static_cast<uint32_t>(m_viewPortSize.y));
        m_hierarchyPanel.SetScene(m_scene);
    }

    void EditorLayer::OpenScene() noexcept
    {
        auto filePath = FileDialogs::OpenFile("Creepy Scene (*.creepy)\0*.creepy\0");

        if (!filePath.empty())
        {
            m_scene = std::make_shared<Scene>(); // Create New Empty Scene
            m_scene->OnViewPortResize(static_cast<uint32_t>(m_viewPortSize.x), static_cast<uint32_t>(m_viewPortSize.y));
            m_hierarchyPanel.SetScene(m_scene);

            SceneSerializer serializer{m_scene};
            serializer.DeserializeFromYaml(filePath);
        }
    }

    void EditorLayer::SaveSceneAs() noexcept
    {
        auto filePath = FileDialogs::SaveFile("Creepy Scene (*.creepy)\0*.creepy\0");

        if (!filePath.empty())
        {
            SceneSerializer serializer{m_scene};
            serializer.SerializeToYaml(filePath);
        }
    }
}