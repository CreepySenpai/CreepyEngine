#include <CreepyEngine/Debug/ImGuiLayer.hpp>
#include <CreepyEngine/Core/Application.hpp>
#include <glad/glad.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace Creepy
{

    ImGuiLayer::ImGuiLayer() noexcept : Layer("ImGuiLayer") {

    }

    void ImGuiLayer::OnAttach() noexcept {
        // ImGui::CreateContext();
        // ImGui::StyleColorsDark();

        // ImGuiIO& io = ImGui::GetIO();
        // io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

        // io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        
        ImGui::StyleColorsDark();
        ENGINE_LOG_INFO("Init create");

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        
        // Be careful when get instance of this obj
        ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow()), true);
            
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void ImGuiLayer::OnDetach() noexcept {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    // void ImGuiLayer::OnUpdate() noexcept {
    //     // auto& io = ImGui::GetIO();
    //     // auto& app = Application::GetInstance();
    //     // io.DisplaySize = ImVec2(app.GetWindow().GetWindowWidth(), app.GetWindow().GetWindowHeight());

    //     // float time = (float)glfwGetTime();

        
    //     // io.DeltaTime = m_time > 0.0f ? (time - m_time) : ( 1.0f / 60.0f);
    //     // m_time = time;


    //     // // We must setup it before call it
    //     // ImGui_ImplOpenGL3_NewFrame();
    //     // ImGui::NewFrame();
        

    //     // // Draw Data Here

    //     // ImGui::Begin("Start");

    //     // ImGui::Text("This is some useful text.");

    //     // static char buffer[512] = "Anh Dep Trai";

    //     // ImGui::InputText("Text", buffer, GetArraySize(buffer));

    //     // ImGui::End();

    //     // ImGui::Render();
    //     // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // }

    void ImGuiLayer::OnImGuiRender() noexcept {

        // ImGui::Begin("Start");

        // ImGui::Text("This is some useful text.");

        // static char buffer[512] = "Anh Dep Trai";

        // ImGui::InputText("Text", buffer, GetArraySize(buffer));

        // ImGui::End();
        
        
    }

    void ImGuiLayer::Begin() noexcept {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End() noexcept {
        auto& io = ImGui::GetIO();
        auto& app = Application::GetInstance();
        io.DisplaySize = ImVec2(app.GetWindow().GetWindowWidth(), app.GetWindow().GetWindowHeight());


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        
    }

    // void ImGuiLayer::OnEvent(Event& event) noexcept {
    //     EventDispatcher dispatcher{event};

    //     // We bind every func and check if current event match type of event func then we handle it
    //     dispatcher.Dispatch<WindowResizeEvent>(std::bind_front(ImGuiLayer::OnWindowResizedEvent, this));

    //     dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind_front(ImGuiLayer::OnMouseButtonPressedEvent, this));

    //     dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind_front(ImGuiLayer::OnMouseButtonReleasedEvent, this));

    //     dispatcher.Dispatch<MouseMovedEvent>(std::bind_front(ImGuiLayer::OnMouseMovedEvent, this));

    //     dispatcher.Dispatch<MouseScrolledEvent>(std::bind_front(ImGuiLayer::OnMouseScrolledEvent, this));

    //     dispatcher.Dispatch<KeyPressedEvent>(std::bind_front(ImGuiLayer::OnKeyPressedEvent, this));

    //     dispatcher.Dispatch<KeyReleasedEvent>(std::bind_front(ImGuiLayer::OnKeyReleasedEvent, this));

    //     dispatcher.Dispatch<KeyTypedEvent>(std::bind_front(ImGuiLayer::OnKeyTypedEvent, this));
    // }


    // // All ImGui Event We Get Just To Get Information To Handle Debug Data, So We Dont Need To Set Handle = true
    // bool ImGuiLayer::OnWindowResizedEvent(WindowResizeEvent &event) noexcept
    // {
    //     auto&& io = ImGui::GetIO();
    //     io.DisplaySize = ImVec2(static_cast<float>(event.GetWindowWidth()), static_cast<float>(event.GetWindowHeight()));
    //     io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    //     glViewport(0, 0, event.GetWindowWidth(), event.GetWindowHeight());

    //     return false;
    // }

    // bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent &event) noexcept
    // {
    //     auto&& io = ImGui::GetIO();
    //     // io.MouseDown[event.GetButton()] = true;
    //     io.AddMouseButtonEvent(event.GetButton(), true);
    //     return false;
    // }

    // bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent &event) noexcept
    // {
    //     auto&& io = ImGui::GetIO();
    //     // io.MouseDown[event.GetButton()] = false;
    //     io.AddMouseButtonEvent(event.GetButton(), false);
    //     return false;
    // }

    // bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent &event) noexcept
    // {
    //     auto&& io = ImGui::GetIO();
    //     // io.MousePos = ImVec2(static_cast<float>(event.GetMouseX()), static_cast<float>(event.GetMouseY()));
    //     io.AddMousePosEvent(static_cast<float>(event.GetMouseX()), static_cast<float>(event.GetMouseY()));
    //     return false;
    // }
    // bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent &event) noexcept
    // {
    //     auto&& io = ImGui::GetIO();
    //     // io.MouseWheel += event.GetYOffset();
    //     // io.MouseWheelH += event.GetXOffset();
    //     io.AddMouseWheelEvent(static_cast<float>(event.GetXOffset()), static_cast<float>(event.GetYOffset()));
    //     return false;
    // }
    // bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent &event) noexcept
    // {
    //     auto&& io = ImGui::GetIO();

    //     io.AddKeyEvent(Creepy_Key_To_ImGui(event.GetKeyCode()), true);
        
    //     io.KeyCtrl = ImGui::IsKeyDown(Creepy_Key_To_ImGui(CREEPY_KEY_LEFT_CONTROL)) 
    //                 || ImGui::IsKeyDown(Creepy_Key_To_ImGui(CREEPY_KEY_RIGHT_CONTROL));

    //     io.KeyShift = ImGui::IsKeyDown(Creepy_Key_To_ImGui(CREEPY_KEY_LEFT_SHIFT)) 
    //                 || ImGui::IsKeyDown(Creepy_Key_To_ImGui(CREEPY_KEY_RIGHT_SHIFT));

    //     io.KeyAlt = ImGui::IsKeyDown(Creepy_Key_To_ImGui(CREEPY_KEY_LEFT_ALT)) 
    //                 || ImGui::IsKeyDown(Creepy_Key_To_ImGui(CREEPY_KEY_RIGHT_ALT));

    //     io.KeySuper = ImGui::IsKeyDown(Creepy_Key_To_ImGui(CREEPY_KEY_LEFT_SUPER)) 
    //                 || ImGui::IsKeyDown(Creepy_Key_To_ImGui(CREEPY_KEY_RIGHT_SUPER));

    //     return false;
    // }
    // bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent &event) noexcept
    // {
    //     auto&& io = ImGui::GetIO();
                
    //     io.AddKeyEvent(Creepy_Key_To_ImGui(event.GetKeyCode()), false);
    //     return false;
    // }
    // bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent &event) noexcept
    // {
    //     auto&& io = ImGui::GetIO();

    //     io.AddInputCharacter(static_cast<uint32_t>(event.GetKeyCode()));

    //     return false;
    // }
}

