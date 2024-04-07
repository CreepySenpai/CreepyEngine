#include <Editor/EditorLayer.hpp>
#include <imgui/ImGuizmo.h>

namespace Creepy {

    extern const std::filesystem::path AssetDirectory;

    static char buffer[256];

    EditorLayer* EditorLayer::s_instance = nullptr;

    EditorLayer::EditorLayer() noexcept : Layer{"LevelEditor"}{
        if(s_instance){
            APP_LOG_ERROR("Only once Editor Instance exit");
        }
        s_instance = this;

        FrameBufferSpecification spec{.Width = 700, .Height = 700, .Attachments = {FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::GREEN_INT, FrameBufferTextureFormat::DEPTH}};
        m_frameBuffer = FrameBuffer::Create(spec);

        // Point to same scene
        m_editorScene = std::make_shared<Scene>();
        m_activeScene = m_editorScene;

        m_playIcon = Texture2D::Create("./assets/icons/play_icon.png");

        m_stopIcon = Texture2D::Create("./assets/icons/stop_icon.png");

        m_simulationIcon = Texture2D::Create("./assets/icons/simulation_icon.png");

        m_editorCamera = EditorCamera{45.0f, 1.0f, 0.01f, 1000.0f};
        

        m_hierarchyPanel.SetScene(m_activeScene);
    }

    EditorLayer::~EditorLayer() noexcept {
        APP_LOG_WARNING("Call Shut Down App");
    }

    void EditorLayer::OnAttach() noexcept {

    }

    void EditorLayer::OnDetach() noexcept {

    }

    void EditorLayer::OnUpdate(TimeStep timeStep) noexcept {
        
        // Resize
        if(FrameBufferSpecification spec = m_frameBuffer->GetSpecification();
            m_viewPortSize.x > 0.0f && m_viewPortSize.y > 0.0f && (spec.Width != static_cast<uint32_t>(m_viewPortSize.x) 
            || spec.Height != static_cast<uint32_t>(m_viewPortSize.y))
        ) {

            m_frameBuffer->Resize(static_cast<uint32_t>(m_viewPortSize.x), static_cast<uint32_t>(m_viewPortSize.y));

            m_editorCamera.SetViewPortSize(static_cast<uint32_t>(m_viewPortSize.x), static_cast<uint32_t>(m_viewPortSize.y));

            m_activeScene->OnViewPortResize(m_viewPortSize.x, m_viewPortSize.y);
        }

        m_frameBuffer->Bind();

        RenderCommand::SetClearColor({182.0f / 255.0f, 200.0f / 255.0f, 231.0f / 255.0f, 1.0f});
        RenderCommand::Clear();
        Renderer::ResetStatistics();
        
        // Clear Color Attachment
        m_frameBuffer->ClearColorBufferAttachment(1, -1);

        switch (m_sceneState)
        {
            case SceneState::EDIT:
                {
                    if(m_viewPortHovered){
                        m_editorCamera.OnUpdate(timeStep);
                    }
                    m_activeScene->OnUpdateEditor(timeStep, m_editorCamera);
                    break;
                }
            case SceneState::SIMULATION:
                {
                    if(m_viewPortHovered){
                        m_editorCamera.OnUpdate(timeStep);
                    }
                    m_activeScene->OnUpdateSimulation(timeStep, m_editorCamera);
                    break;
                }
            case SceneState::PLAY:
                m_activeScene->OnUpdateRunTime(timeStep);
                break;
        }


        {   // Mouse Picking
            auto [mX, mY] = ImGui::GetMousePos();

            mX -= m_viewPortBounds[0].x;
            mY -= m_viewPortBounds[0].y;

            const glm::vec2 viewPortSize{m_viewPortBounds[1] - m_viewPortBounds[0]};

            // Flip Coord From Top Left -> Bottom Left To Match Texture Coord
            mY = viewPortSize.y - mY;
            int mouseXInViewPort = static_cast<int>(mX);
            int mouseYInViewPort = static_cast<int>(mY);
            
            if(mouseXInViewPort > 0 && mouseYInViewPort > 0 && mouseXInViewPort < viewPortSize.x && mouseYInViewPort < viewPortSize.y){
                int entityID = m_frameBuffer->ReadPixel(1, mouseXInViewPort, mouseYInViewPort);
                if(entityID == -1){
                    m_hoverEntity = {};
                } else {
                    m_hoverEntity = {static_cast<entt::entity>(entityID), m_activeScene.get()};
                }
            }
        }

        {
            
            this->onOverlayRender();
        }
        
        m_frameBuffer->UnBind();
    }

    void EditorLayer::OnImGuiRender() noexcept {
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
        
        ImGui::Begin("DockSpace Demo", nullptr, window_flags);

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
                    this->newScene();
                }

                ImGui::Separator();

                if(ImGui::MenuItem("Open...", "Ctrl+O")){
                    this->openScene();
                }

                ImGui::Separator();

                if(ImGui::MenuItem("Save", "Ctrl++S")){
                    this->saveScene();
                }

                ImGui::Separator();

                if(ImGui::MenuItem("Save As...", "Ctrl+Shift+S")){
                    this->saveSceneAs();
                }

                ImGui::Separator();

                if(ImGui::MenuItem("Save Theme")){
                    auto filePath = FileDialogs::OpenFile("Creepy Theme (*.mlemlem)\0*.mlemlem\0");
                    if(!filePath.empty()){
                        Application::GetInstance().GetImGuiLayer().SaveThemeToYaml(filePath);
                    }
                }

                ImGui::Separator();

                if(ImGui::MenuItem("Load Theme")){
                    auto filePath = FileDialogs::SaveFile("Creepy Theme (*.mlemlem)\0*.mlemlem\0");
                    if(!filePath.empty()){

                        if(Application::GetInstance().GetImGuiLayer().LoadThemeFromYaml(filePath)){
                            Application::GetInstance().GetImGuiLayer().SetTheme();
                        }
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

        auto&& viewPortMinRegion = ImGui::GetWindowContentRegionMin();
        auto&& viewPortMaxRegion = ImGui::GetWindowContentRegionMax();
        auto&& viewPortOffset = ImGui::GetWindowPos();

        m_viewPortBounds[0] = {viewPortMinRegion.x + viewPortOffset.x, viewPortMinRegion.y + viewPortOffset.y};
        m_viewPortBounds[1] = {viewPortMaxRegion.x + viewPortOffset.x, viewPortMaxRegion.y + viewPortOffset.y};

        m_viewPortFocused = ImGui::IsWindowFocused();
        m_viewPortHovered = ImGui::IsWindowHovered();

        Application::GetInstance().GetImGuiLayer().BlockEvents(!m_viewPortFocused && !m_viewPortHovered);

        auto&& viewPortSize = ImGui::GetContentRegionAvail();

        // // Check if view port change we resize it
        if((m_viewPortSize.x != viewPortSize.x) || (m_viewPortSize.y != viewPortSize.y)) {
            m_viewPortSize.x = viewPortSize.x;
            m_viewPortSize.y = viewPortSize.y;
        }
        

        auto id = m_frameBuffer->GetColorAttachmentID();
        ImGui::Image(reinterpret_cast<ImTextureID>(id), ImVec2{m_viewPortSize.x, m_viewPortSize.y}, ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});
        
        if(ImGui::BeginDragDropTarget()){
            
            // Payload maybe null
            if(auto payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")){
                auto path = reinterpret_cast<const char*>(payload->Data);

                this->openScene(AssetDirectory / path);
            }

            ImGui::EndDragDropTarget();
        }
        // Gizmos

        this->drawGizmos();

        ImGui::End();
        ImGui::PopStyleVar();

        m_hierarchyPanel.OnImGuiRender();

        m_contentBrowserPanel.OnImGuiRender();

        ImGui::Begin("Statics");

        auto stats = Creepy::Renderer::GetStatistics();

        std::string hoveredName ="None";
        if(m_hoverEntity.IsExits()){
            hoveredName.assign(m_hoverEntity.GetComponent<TagComponent>().Tag);
        }
        ImGui::Text("Hover Entity %s", hoveredName.c_str());
        ImGui::Text("Render2D Stats");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Rect Count: %d", stats.RectCount);
        ImGui::Text("Total Vertex: %d", stats.GetTotalVertexCount());
        ImGui::Text("Total Index: %d", stats.GetTotalIndexCount());
        ImGui::Text("Camera Position: %f , %f , %f", m_editorCamera.GetPosition().x, m_editorCamera.GetPosition().y, m_editorCamera.GetPosition().z);
        ImGui::Checkbox("Show Physic Collider", &m_showPhysicCollider);
        ImGui::End();

        {
            this->drawThemePanel();
        }

        {
             this->uiDrawToolBar();
        }

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event &event) noexcept {
        if(m_viewPortFocused && m_viewPortHovered){
            m_editorCamera.OnEvent(event);   
        }

        EventDispatcher dispatcher{event};

        dispatcher.Dispatch<KeyPressedEvent>(std::bind_front(onKeyPressed, this));
        dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind_front(onMouseButtonPressed, this));
    }

    bool EditorLayer::onKeyPressed(KeyPressedEvent& event) noexcept {
        if(event.IsRepeat()){
            return false;
        }

        const bool ctrl = Input::IsKeyPressed(KeyCode::KEY_LEFT_CONTROL) || Input::IsKeyPressed(KeyCode::KEY_RIGHT_CONTROL);
        const bool shift = Input::IsKeyPressed(KeyCode::KEY_LEFT_SHIFT) || Input::IsKeyPressed(KeyCode::KEY_RIGHT_SHIFT);

        switch (event.GetKeyCode())
        {
            case KeyCode::KEY_N: {
                if(ctrl){
                    this->newScene();
                }
                break;
            }

            case KeyCode::KEY_O: {
                if(ctrl){
                    this->openScene();
                }
                break;
            }

            case KeyCode::KEY_S: {

                if(ctrl){
                    if(shift){
                        this->saveSceneAs();
                    } else {
                        this->saveScene();
                    }
                }
                
                break;
            }

            case KeyCode::KEY_D: {
                if(ctrl){
                    this->onDuplicateEntity();
                }
                break;
            }

            case KeyCode::KEY_Q: {
                m_gizmosType = -1;
                break;
            }
            case KeyCode::KEY_W: {
                m_gizmosType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            }
            case KeyCode::KEY_E: {
                m_gizmosType = ImGuizmo::OPERATION::ROTATE;
                break;
            }
            case KeyCode::KEY_R: {
                m_gizmosType = ImGuizmo::OPERATION::SCALE;
                break;
            }
        }

        return false;
    }

    SceneState EditorLayer::GetEditorState() const noexcept {
        return m_sceneState;
    }

    EditorLayer& EditorLayer::GetInstance() noexcept {
        return *s_instance;
    }

    bool EditorLayer::onMouseButtonPressed(MouseButtonPressedEvent& event) noexcept {
        if(event.GetButton() == MouseButtonCode::MOUSE_LEFT && this->canMousePicking()){
            m_hierarchyPanel.SetSelectedEntity(m_hoverEntity);
        }

        return false;
    }

    void EditorLayer::onOverlayRender() noexcept {
        
        if(m_sceneState == SceneState::PLAY){
            Entity camera = m_activeScene->GetPrimaryCameraEntity();

            if(!camera.IsExits()){
                return;
            }
            Renderer::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
        }
        else {
            Renderer::BeginScene(m_editorCamera);
        }
        
        if(m_showPhysicCollider){
            m_activeScene->GetAllEntitiesType<TransformComponent, CircleCollider2DComponent>().each([editorDepth = m_editorCamera.GetPosition().z](auto entityID, TransformComponent& transformComponent, CircleCollider2DComponent& circleCollider2DComponent){
                glm::vec3 debugPosition = transformComponent.Position + glm::vec3{circleCollider2DComponent.Offset, editorDepth > transformComponent.Position.z ? 0.001f : -0.001f};

                glm::vec3 debugScale = transformComponent.Scale * glm::vec3{circleCollider2DComponent.Radius * 2.0f};

                glm::mat4 transform = glm::translate(glm::mat4{1.0f}, debugPosition) * glm::scale(glm::mat4{1.0f}, debugScale);

                Renderer::DrawCircle(transform, glm::vec4{0.0f, 1.0f, 0.0f, 1.0f}, 0.1f);
            });

            m_activeScene->GetAllEntitiesType<TransformComponent, BoxCollider2DComponent>().each([editorDepth = m_editorCamera.GetPosition().z](auto entityID, TransformComponent& transformComponent, BoxCollider2DComponent& boxC2DComp){
                glm::vec3 debugPosition = transformComponent.Position + glm::vec3{boxC2DComp.Offset.x, boxC2DComp.Offset.y , editorDepth > transformComponent.Position.z ? 0.001f : -0.001f};
                glm::vec3 debugScale = transformComponent.Scale * glm::vec3{boxC2DComp.Size * 2.0f, 1.0f};
                
                glm::mat4 transform = glm::translate(glm::mat4{1.0f}, debugPosition) 
                    * glm::rotate(glm::mat4{1.0f}, transformComponent.Rotation.z, glm::vec3{0.0f, 0.0f, 1.0f})
                    * glm::scale(glm::mat4{1.0f}, debugScale);

                Renderer::DrawLineRect(transform, {0.0f, 1.0f, 0.0f, 1.0f});
            });
        }

        if(Entity& selectedEntity = m_hierarchyPanel.GetSelectedEntity(); selectedEntity.IsExits()){

            Renderer::DrawLineRect(selectedEntity.GetComponent<TransformComponent>().GetTransform(), {1.0f, 0.0f, 0.0f, 1.0f});

        }

        Renderer::EndScene();
    }

    void EditorLayer::onScenePlay() noexcept {

        if(m_sceneState == SceneState::SIMULATION){
            this->onSceneStop();
        }

        m_sceneState = SceneState::PLAY;

        // Copy data to new scene
        m_activeScene = Scene::Copy(m_editorScene); // Copy Editor Scene

        m_activeScene->OnRuntimePlay();

        m_hierarchyPanel.SetScene(m_activeScene);
    }

    void EditorLayer::onSimulationPlay() noexcept {

        if(m_sceneState == SceneState::PLAY){
            this->onSceneStop();
        }

        m_sceneState = SceneState::SIMULATION;

        // Copy data to new scene
        m_activeScene = Scene::Copy(m_editorScene); // Copy Editor Scene

        m_activeScene->OnSimulationPlay();

        m_hierarchyPanel.SetScene(m_activeScene);
    }

    void EditorLayer::onSceneStop() noexcept {

        if(m_sceneState == SceneState::PLAY){
            m_activeScene->OnRuntimeStop();
        }
        else if(m_sceneState == SceneState::SIMULATION) {
            m_activeScene->OnSimulationStop();
        }

        m_sceneState = SceneState::EDIT;

        // Reset Runtime
        m_activeScene.reset();
        m_activeScene = m_editorScene;
        m_hierarchyPanel.SetScene(m_activeScene);

    }

    void EditorLayer::onDuplicateEntity() noexcept {
        if(m_sceneState != SceneState::EDIT){
            return;
        }

        Entity entity = m_hierarchyPanel.GetSelectedEntity();
        if(entity.IsExits()){
            m_editorScene->DuplicateEntity(entity);
        }
    }

    void EditorLayer::uiDrawToolBar() noexcept {
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 2.0f});
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, {0.0f, 0.0f});
        ImGui::PushStyleColor(ImGuiCol_Button, {0.0f, 0.0f, 0.0f, 0.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.3f, 0.305f, 0.31f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.15f, 0.12f, 0.16f, 1.f});

        ImGui::Begin("##toolBar", nullptr, 
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        
        {
            auto iconID = reinterpret_cast<ImTextureID>(m_sceneState == SceneState::EDIT 
                    || m_sceneState == SceneState::SIMULATION ? m_playIcon->GetRendererID() : m_stopIcon->GetRendererID());

            const float iconSize{ImGui::GetWindowHeight() - 2.0f};  // Padding

            // Center Icon
            ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.5f) - (iconSize * 0.5f));

            if(ImGui::ImageButton(iconID, {iconSize, iconSize}, {0, 0}, {1, 1}, 0)){

                if(m_sceneState == SceneState::EDIT || m_sceneState == SceneState::SIMULATION){
                    this->onScenePlay();
                } else if(m_sceneState == SceneState::PLAY){
                    this->onSceneStop();
                }
            
            }
        }

        ImGui::SameLine();

        {
            auto iconID = reinterpret_cast<ImTextureID>(m_sceneState == SceneState::EDIT 
                    || m_sceneState == SceneState::PLAY ? m_simulationIcon->GetRendererID() : m_stopIcon->GetRendererID());

            const float iconSize{ImGui::GetWindowHeight() - 2.0f};  // Padding

            if(ImGui::ImageButton(iconID, {iconSize, iconSize}, {0, 0}, {1, 1}, 0)){

                if(m_sceneState == SceneState::EDIT || m_sceneState == SceneState::PLAY){
                    this->onSimulationPlay();
                } else if(m_sceneState == SceneState::SIMULATION){
                    this->onSceneStop();
                }
            
            }
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);

        ImGui::End();
    }

    void EditorLayer::drawGizmos() noexcept {
        auto&& selectedEntity = m_hierarchyPanel.GetSelectedEntity();

        if(selectedEntity.IsExits() && m_gizmosType != -1){
            ImGuizmo::SetOrthographic(true);
            ImGuizmo::SetDrawlist();
            
            const float windowWidth = ImGui::GetWindowWidth();
            const float windowHeight = ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

                // auto cameraEntity = m_scene->GetPrimaryCameraEntity();
                // Camera
                // auto&& cameraComp = cameraEntity.GetComponent<CameraComponent>();
                // auto&& cameraProjection = cameraComp.Camera.GetProjection();
                // glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

                //Editor Camera
                const auto& cameraProjectionMatrix = m_editorCamera.GetProjection();
                const auto& cameraViewMatrix = m_editorCamera.GetViewMatrix();
                // Current Entity
                auto&& entityTransform = selectedEntity.GetComponent<TransformComponent>();
                glm::mat4 transform = entityTransform.GetTransform();
                
                const bool isSnapping = Input::IsKeyPressed(KeyCode::KEY_LEFT_CONTROL);
                const float snapValue = m_gizmosType == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f;
                
                const float snapValues[] {snapValue, snapValue, snapValue};


                ImGuizmo::Manipulate(glm::value_ptr(cameraViewMatrix), glm::value_ptr(cameraProjectionMatrix), 
                    static_cast<ImGuizmo::OPERATION>(m_gizmosType), ImGuizmo::MODE::LOCAL, glm::value_ptr(transform),
                    nullptr, isSnapping ? snapValues : nullptr);

                
                if(ImGuizmo::IsUsing()){
                    glm::vec3 position, rotation, scale;

                    Math::DecomposeTransform(transform, position, rotation, scale);
                    
                    entityTransform.Position = position;
                    
                    // To prevent gimbal lock
                    auto deltaRotation = rotation - entityTransform.Rotation;
                    entityTransform.Rotation += deltaRotation;

                    entityTransform.Scale = scale;
                }
        }
    }

    void EditorLayer::drawThemePanel() noexcept {

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

    void EditorLayer::newScene() noexcept {
        if(m_sceneState != SceneState::EDIT){
            this->onSceneStop();
        }

        m_editorScene.reset();
        m_editorScene = std::make_shared<Scene>();

        m_activeScene.reset();
        m_activeScene = m_editorScene;

        m_hierarchyPanel.SetScene(m_activeScene);

        m_activeScene->OnViewPortResize(static_cast<uint32_t>(m_viewPortSize.x), static_cast<uint32_t>(m_viewPortSize.y));

        m_editorCamera.SetViewPortSize(static_cast<uint32_t>(m_viewPortSize.x), static_cast<uint32_t>(m_viewPortSize.y));
        
        m_editorScenePath.clear();
        
    }

    void EditorLayer::openScene() noexcept
    {
        auto filePath = FileDialogs::OpenFile("Creepy Scene (*.creepy)\0*.creepy\0");

        if (!filePath.empty())
        {
            this->openScene(filePath);
        }
    }

    void EditorLayer::openScene(const std::filesystem::path& filePath) noexcept {
        
        // TODO: Add Dialog to check user
        if(m_sceneState != SceneState::EDIT){
            this->onSceneStop();
        }

        if(std::filesystem::exists(filePath) && filePath.extension().string() == ".creepy"){
            
            Ref<Scene> newScene = std::make_shared<Scene>();
            SceneSerializer serializer{newScene};

            if(serializer.DeserializeFromYaml(filePath.string())){

                m_editorScene.reset();
                m_editorScene = newScene;
                m_activeScene.reset();
                m_activeScene = m_editorScene; // Create New Empty Scene

                m_hierarchyPanel.SetScene(m_activeScene);

                m_activeScene->OnViewPortResize(static_cast<uint32_t>(m_viewPortSize.x), static_cast<uint32_t>(m_viewPortSize.y));
                m_editorCamera.SetViewPortSize(static_cast<uint32_t>(m_viewPortSize.x), static_cast<uint32_t>(m_viewPortSize.y));
                
                m_editorScenePath = filePath;

            }
            
        }
        
    }

    void EditorLayer::saveScene() noexcept {
        if(std::filesystem::exists(m_editorScenePath)){
            SceneSerializer serializer{m_activeScene};
            serializer.SerializeToYaml(m_editorScenePath);
        }
        else {
            this->saveSceneAs();
        }
    }

    void EditorLayer::saveSceneAs() noexcept
    {
        if(m_sceneState != SceneState::EDIT){
            this->onSceneStop();
        }
        auto filePath = FileDialogs::SaveFile("Creepy Scene (*.creepy)\0*.creepy\0");

        if (!filePath.empty())
        {
            SceneSerializer serializer{m_activeScene};
            serializer.SerializeToYaml(filePath);
            m_editorScenePath = filePath;
        }
    }

    bool EditorLayer::canMousePicking() noexcept {
        return m_viewPortHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(KeyCode::KEY_LEFT_ALT);
    }

    void EditorLayer::duplicateEntity(Entity& entity) noexcept {
        Entity newEntity = m_activeScene->CreateEntity(entity.GetName());

    }
}