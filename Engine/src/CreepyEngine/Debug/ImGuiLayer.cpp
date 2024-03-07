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

    void ImGuiLayer::OnImGuiRender() noexcept {    
        
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

    void ImGuiLayer::OnEvent(Event& event) noexcept {
        
        // Some event handle by ImGui so we don't need send it to application
        if(m_blockEvents){
            auto& io = ImGui::GetIO();
            event.Handled |= event.IsInCategory(EventCategory::MOUSE) & io.WantCaptureMouse;
            event.Handled |= event.IsInCategory(EventCategory::KEYBOARD) & io.WantCaptureKeyboard;
        }


    }
}

