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

    void ImGuiLayer::OnUpdate(TimeStep timeStep) noexcept {
        this->SetTheme();
    }

    void ImGuiLayer::OnImGuiRender() noexcept {   
        ImGui::Begin("Theme Setting");

            ImGui::ColorEdit4("Window Background", glm::value_ptr(m_editorConfig.WindowBg));
            ImGui::ColorEdit4("Header", glm::value_ptr(m_editorConfig.Header));
            ImGui::ColorEdit4("Header Hovered", glm::value_ptr(m_editorConfig.HeaderHovered));
            ImGui::ColorEdit4("Header Active", glm::value_ptr(m_editorConfig.HeaderActive));

            ImGui::ColorEdit4("Button", glm::value_ptr(m_editorConfig.Button));
            ImGui::ColorEdit4("Button Hovered", glm::value_ptr(m_editorConfig.ButtonHovered));
            ImGui::ColorEdit4("Button Active", glm::value_ptr(m_editorConfig.ButtonActive));

            ImGui::ColorEdit4("Frame Background", glm::value_ptr(m_editorConfig.FrameBg));
            ImGui::ColorEdit4("FrameBg Hovered", glm::value_ptr(m_editorConfig.FrameBgHovered));
            ImGui::ColorEdit4("FrameBg Active", glm::value_ptr(m_editorConfig.FrameBgActive));

            ImGui::ColorEdit4("Tab", glm::value_ptr(m_editorConfig.Tab));
            ImGui::ColorEdit4("Tab Hovered", glm::value_ptr(m_editorConfig.TabHovered));
            ImGui::ColorEdit4("Tab Active", glm::value_ptr(m_editorConfig.TabActive));
            ImGui::ColorEdit4("Tab Unfocused", glm::value_ptr(m_editorConfig.TabUnfocused));
            ImGui::ColorEdit4("Tab Unfocused Active", glm::value_ptr(m_editorConfig.TabUnfocusedActive));


            ImGui::End();
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
        colors[ImGuiCol_WindowBg] = ImVec4{m_editorConfig.WindowBg.r, m_editorConfig.WindowBg.g, m_editorConfig.WindowBg.b, m_editorConfig.WindowBg.a};

		// Headers
		colors[ImGuiCol_Header] = ImVec4{m_editorConfig.Header.r, m_editorConfig.Header.g, m_editorConfig.Header.b, m_editorConfig.Header.a};

		colors[ImGuiCol_HeaderHovered] = ImVec4{m_editorConfig.HeaderHovered.r, m_editorConfig.HeaderHovered.g, m_editorConfig.HeaderHovered.b, m_editorConfig.HeaderHovered.a};
		colors[ImGuiCol_HeaderActive] = ImVec4{ m_editorConfig.HeaderActive.r, m_editorConfig.HeaderActive.g, m_editorConfig.HeaderActive.b, m_editorConfig.HeaderActive.a};
		
        ENGINE_LOG_WARNING("Call change button color: R:{}, G:{}, B:{}, A:{}", m_editorConfig.Button.r, m_editorConfig.Button.g, m_editorConfig.Button.b, m_editorConfig.Button.a);
		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ m_editorConfig.Button.r, m_editorConfig.Button.g, m_editorConfig.Button.b, m_editorConfig.Button.a };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ m_editorConfig.ButtonHovered.r, m_editorConfig.ButtonHovered.g, m_editorConfig.ButtonHovered.b, m_editorConfig.ButtonHovered.a };
		colors[ImGuiCol_ButtonActive] = ImVec4{ m_editorConfig.ButtonActive.r, m_editorConfig.ButtonActive.g, m_editorConfig.ButtonActive.b, m_editorConfig.ButtonActive.a };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ m_editorConfig.FrameBg.r, m_editorConfig.FrameBg.g, m_editorConfig.FrameBg.b, m_editorConfig.FrameBg.a  };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ m_editorConfig.FrameBgHovered.r, m_editorConfig.FrameBgHovered.g, m_editorConfig.FrameBgHovered.b, m_editorConfig.FrameBgHovered.a  };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ m_editorConfig.FrameBgActive.r, m_editorConfig.FrameBgActive.g, m_editorConfig.FrameBgActive.b, m_editorConfig.FrameBgActive.a  };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ m_editorConfig.Tab.r, m_editorConfig.Tab.g, m_editorConfig.Tab.b, m_editorConfig.Tab.a };
		colors[ImGuiCol_TabHovered] = ImVec4{ m_editorConfig.TabHovered.r, m_editorConfig.TabHovered.g, m_editorConfig.TabHovered.b, m_editorConfig.TabHovered.a };
		colors[ImGuiCol_TabActive] = ImVec4{ m_editorConfig.TabActive.r, m_editorConfig.TabActive.g, m_editorConfig.TabActive.b, m_editorConfig.TabActive.a };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ m_editorConfig.TabUnfocused.r, m_editorConfig.TabUnfocused.g, m_editorConfig.TabUnfocused.b, m_editorConfig.TabUnfocused.a };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ m_editorConfig.TabUnfocusedActive.r, m_editorConfig.TabUnfocusedActive.g, m_editorConfig.TabUnfocusedActive.b, m_editorConfig.TabUnfocusedActive.a  };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ m_editorConfig.TitleBg.r, m_editorConfig.TitleBg.g, m_editorConfig.TitleBg.b, m_editorConfig.TitleBg.a };
		colors[ImGuiCol_TitleBgActive] = ImVec4{  m_editorConfig.TitleBgActive.r, m_editorConfig.TitleBgActive.g, m_editorConfig.TitleBgActive.b, m_editorConfig.TitleBgActive.a };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ m_editorConfig.TitleBgCollapse.r, m_editorConfig.TitleBgCollapse.g, m_editorConfig.TitleBgCollapse.b, m_editorConfig.TitleBgCollapse.a };

    }
}

