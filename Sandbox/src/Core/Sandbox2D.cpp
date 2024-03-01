#include <chrono>
#include <Core/Sandbox2D.hpp>

template <typename Func>
class TimeCal{

    public:

        TimeCal(const char* name, Func&& func) : m_name{name}, m_func{func}, m_stop{false}{
            m_startPoint = std::chrono::steady_clock::now();
        }

        void Stop(){
            auto endPoint = std::chrono::steady_clock::now();

            auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startPoint).time_since_epoch().count();

            auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();
            
            m_stop = true;

            float duration = (end - start) * 0.001f;

            m_func({m_name, duration});
        }

        ~TimeCal(){
            if(!m_stop){
                Stop();
            }
        }
    
    private:
        const char* m_name;
        std::chrono::time_point<std::chrono::steady_clock> m_startPoint;
        Func m_func;
        bool m_stop;
};

Sandbox2D::Sandbox2D() noexcept : Creepy::Layer{"SandBox"}, m_cameraController{600.0f / 600.0f, true}{
    Creepy::Renderer::Init();
    m_texture = Creepy::Texture2D::Create("./assets/textures/SpecularMap.png");
}

void Sandbox2D::OnAttach() noexcept
{

}

void Sandbox2D::OnDetach() noexcept
{
    
}

void Sandbox2D::OnUpdate(const Creepy::TimeStep &timeStep) noexcept
{
    TimeCal time("UpdateTime", [this](const ProfileResult& result) {
        m_profiles.push_back(result);
    });

    m_cameraController.OnUpdate(timeStep);

    Creepy::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    Creepy::RenderCommand::Clear();

    Creepy::Renderer2D::BeginScene(m_cameraController.GetCamera());

    Creepy::Renderer2D::DrawRect({1.0f, 2.0f, 1.0f}, {5.f, 5.f}, m_playerColor);

    Creepy::Renderer2D::DrawRect({6.0f, 7.0f, 1.0f}, {10.f, 5.f}, m_texture);

    Creepy::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender() noexcept
{
    ImGui::Begin("Color Picker");
    ImGui::ColorEdit4("My Color", glm::value_ptr(m_playerColor));

    for(auto& profile : m_profiles){
        ImGui::Text("%s : %.3f ms", profile.Name, profile.Duration);
    }

    m_profiles.clear();
    ImGui::End();
}

void Sandbox2D::OnEvent(Creepy::Event &event) noexcept
{
    m_cameraController.OnEvent(event);
}