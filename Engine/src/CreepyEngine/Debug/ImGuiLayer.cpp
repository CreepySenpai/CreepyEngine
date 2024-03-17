#include <CreepyEngine/Debug/ImGuiLayer.hpp>
#include <CreepyEngine/Core/Application.hpp>
#include <glad/glad.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <yaml-cpp/yaml_ex.hpp>

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
        colors[ImGuiCol_WindowBg] = ImVec4{m_editorConfig.WindowBg.r, m_editorConfig.WindowBg.g, m_editorConfig.WindowBg.b, m_editorConfig.WindowBg.a};

		// Headers
		colors[ImGuiCol_Header] = ImVec4{m_editorConfig.Header.r, m_editorConfig.Header.g, m_editorConfig.Header.b, m_editorConfig.Header.a};

		colors[ImGuiCol_HeaderHovered] = ImVec4{m_editorConfig.HeaderHovered.r, m_editorConfig.HeaderHovered.g, m_editorConfig.HeaderHovered.b, m_editorConfig.HeaderHovered.a};
		colors[ImGuiCol_HeaderActive] = ImVec4{ m_editorConfig.HeaderActive.r, m_editorConfig.HeaderActive.g, m_editorConfig.HeaderActive.b, m_editorConfig.HeaderActive.a};
		
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

    void ImGuiLayer::SaveThemeToYaml(const std::string& filePath) noexcept {
        YAML::Emitter writer;

        writer << YAML::BeginMap;
        writer << YAML::Key << "Theme" << YAML::Value << "Unnamed";

        writer << YAML::Key << "Config" << YAML::BeginMap;

        {
            
            writer << YAML::Key << "WindowBg" << YAML::Value << m_editorConfig.WindowBg;

            writer << YAML::Key << "Header" << YAML::Value << m_editorConfig.Header;

            writer << YAML::Key << "HeaderHovered" << YAML::Value << m_editorConfig.HeaderHovered;
            writer << YAML::Key << "HeaderActive" << YAML::Value << m_editorConfig.HeaderActive;

            writer << YAML::Key << "Button" << YAML::Value << m_editorConfig.Button;
            writer << YAML::Key << "ButtonHovered" << YAML::Value << m_editorConfig.ButtonHovered;
            writer << YAML::Key << "ButtonActive" << YAML::Value << m_editorConfig.ButtonActive;

            writer << YAML::Key << "FrameBg" << YAML::Value << m_editorConfig.FrameBg;
            writer << YAML::Key << "FrameBgHovered" << YAML::Value << m_editorConfig.FrameBgHovered;
            writer << YAML::Key << "FrameBgActive" << YAML::Value << m_editorConfig.FrameBgActive;

            writer << YAML::Key << "Tab" << YAML::Value << m_editorConfig.Tab;
            writer << YAML::Key << "TabHovered" << YAML::Value << m_editorConfig.TabHovered;
            writer << YAML::Key << "TabActive" << YAML::Value << m_editorConfig.TabActive;
            writer << YAML::Key << "TabUnfocused" << YAML::Value << m_editorConfig.TabUnfocused;
            writer << YAML::Key << "TabUnfocusedActive" << YAML::Value << m_editorConfig.TabUnfocusedActive;

            writer << YAML::Key << "TitleBg" << YAML::Value << m_editorConfig.TitleBg;
            writer << YAML::Key << "TitleBgActive" << YAML::Value << m_editorConfig.TitleBgActive;
            writer << YAML::Key << "TitleBgCollapse" << YAML::Value << m_editorConfig.TitleBgCollapse;

            writer << YAML::EndMap;
        }

        writer << YAML::EndMap;

        std::ofstream outFile{filePath};
        outFile << writer.c_str();
        outFile.close();
    }

    bool ImGuiLayer::LoadThemeFromYaml(const std::string& filePath) noexcept {
        std::ifstream stream{filePath};
        std::stringstream strStream;
        strStream << stream.rdbuf();

        auto&& themeData = YAML::Load(strStream.str());

        if(!themeData["Theme"]){
            ENGINE_LOG_ERROR("Root Error");
            return false;
        }

        auto&& config = themeData["Config"];

        if(config){
            m_editorConfig.WindowBg = config["WindowBg"].as<glm::vec4>();

            m_editorConfig.Header = config["Header"].as<glm::vec4>();
            m_editorConfig.HeaderHovered = config["HeaderHovered"].as<glm::vec4>();
            m_editorConfig.HeaderActive = config["HeaderActive"].as<glm::vec4>();

            m_editorConfig.Button = config["Button"].as<glm::vec4>();
            m_editorConfig.ButtonHovered = config["ButtonHovered"].as<glm::vec4>();
            m_editorConfig.ButtonActive = config["ButtonActive"].as<glm::vec4>();

            m_editorConfig.FrameBg = config["FrameBg"].as<glm::vec4>();
            m_editorConfig.FrameBgHovered = config["FrameBgHovered"].as<glm::vec4>();
            m_editorConfig.FrameBgActive = config["FrameBgActive"].as<glm::vec4>();

            m_editorConfig.Tab = config["Tab"].as<glm::vec4>();
            m_editorConfig.TabHovered = config["TabHovered"].as<glm::vec4>();
            m_editorConfig.TabActive = config["TabActive"].as<glm::vec4>();
            m_editorConfig.TabUnfocused = config["TabUnfocused"].as<glm::vec4>();
            m_editorConfig.TabUnfocusedActive = config["TabUnfocusedActive"].as<glm::vec4>();

            m_editorConfig.TitleBg = config["TitleBg"].as<glm::vec4>();
            m_editorConfig.TitleBgActive = config["TitleBgActive"].as<glm::vec4>();
            m_editorConfig.TitleBgCollapse = config["TitleBgCollapse"].as<glm::vec4>();
        }
        else {
            ENGINE_LOG_ERROR("Config Error");
        }

        return true;
    }
}

