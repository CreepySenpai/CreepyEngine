#include <iostream>
#include <CreepyEngine.hpp>

class SandboxApplication : public Creepy::Application {
    public:
        SandboxApplication() noexcept{
            std::clog << "Wow\n";
            // this->PushLayer(new Creepy::ImGuiLayer());
        }

        virtual ~SandboxApplication() noexcept {
            
        }
    private:
};

std::unique_ptr<Creepy::Application> Creepy::CreateApplication() {
    return std::make_unique<SandboxApplication>();
}