#include <Editor/EditorLayer.hpp>


namespace Creepy {


    EditorLayer::EditorLayer() noexcept : Layer{"LevelEditor"}, m_cameraController{1.0f} {
        Renderer::Init();

        FrameBufferSpecification spec{.Width = 700, .Height = 700};
        m_frameBuffer = FrameBuffer::Create(spec);

        m_texture = Texture2D::Create("./assets/textures/SpecularMap.png");
    }

    EditorLayer::~EditorLayer() noexcept {
        Renderer::ShutDown();
    }

    void EditorLayer::OnAttach() noexcept {

    }

    void EditorLayer::OnDetach() noexcept {

    }

    void EditorLayer::OnUpdate(const TimeStep &timeStep) noexcept {
        m_cameraController.OnUpdate(timeStep);

        m_frameBuffer->Bind();

        RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
        RenderCommand::Clear();

        Renderer2D::BeginScene(m_cameraController.GetCamera());

        Renderer2D::DrawRect({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, m_texture);

        Renderer2D::EndScene();

        m_frameBuffer->UnBind();
    }

    void EditorLayer::OnImGuiRender() noexcept {
        static bool p_open = false;
        static bool opt_fullscreen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
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
            if (ImGui::BeginMenu("Options"))
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

        auto viewPortSize = ImGui::GetContentRegionAvail();

        // Check if view port change we resize it
        if((m_viewPortSize.x != viewPortSize.x) || (m_viewPortSize.y != viewPortSize.y)) {
            m_viewPortSize.x = viewPortSize.x;
            m_viewPortSize.y = viewPortSize.y;

            m_frameBuffer->Resize(static_cast<uint32_t>(m_viewPortSize.x), static_cast<uint32_t>(m_viewPortSize.y));

            m_cameraController.OnResize(m_viewPortSize.x, m_viewPortSize.y);
        }
        

        // auto id = m_texture->GetRendererID();
        auto id = m_frameBuffer->GetColorAttachmentID();
        ImGui::Image((void*)id, ImVec2{viewPortSize.x, viewPortSize.y}, ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});
        ImGui::End();
        ImGui::PopStyleVar();


        ImGui::End();
    }

    void EditorLayer::OnEvent(Event &event) noexcept {
        m_cameraController.OnEvent(event);
    }
}