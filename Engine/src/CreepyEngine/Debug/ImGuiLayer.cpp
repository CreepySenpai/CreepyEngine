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

        //TODO: Make Font Manager To Use Font Without Remember Index
        io.Fonts->AddFontFromFileTTF("./assets/fonts/Exo2/Exo2-SemiBold.ttf", 18.0f);
        io.FontDefault = io.Fonts->AddFontFromFileTTF("./assets/fonts/Exo2/Exo2-Regular.ttf", 18.0f);
        
        ImGui::StyleColorsDark();
        ENGINE_LOG_INFO("Init create");

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        
        // Config theme
        this->SetTheme();
        
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

    void ImGuiLayer::SetTheme() noexcept {
        auto& colors = ImGui::GetStyle().Colors;

        //Background
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // colors[ImGuiCol_Header] = ImVec4{ 230.f / 255.f, 141.0f / 255.0f, 224.0f / 255.f, 1.0f };
		// colors[ImGuiCol_HeaderHovered] = ImVec4{ 230.f / 255.f, 141.0f / 255.0f, 224.0f / 255.f, 1.0f };
		// colors[ImGuiCol_HeaderActive] = ImVec4{ 230.f / 255.f, 141.0f / 255.0f, 224.0f / 255.f, 1.0f };
		
		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		// colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		// colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		// colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		// colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		// colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        colors[ImGuiCol_Tab] = ImVec4{ 230.f / 255.f, 141.0f / 255.0f, 224.0f / 255.f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 230.f / 255.f, 141.0f / 255.0f, 224.0f / 255.f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 230.f / 255.f, 141.0f / 255.0f, 224.0f / 255.f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 230.f / 255.f, 141.0f / 255.0f, 224.0f / 255.f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 230.f / 255.f, 141.0f / 255.0f, 224.0f / 255.f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    }
}

