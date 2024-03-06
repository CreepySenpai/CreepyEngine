#include <iostream>
#include <CreepyEngine.hpp>
#include <CreepyEngine/Core/EntryPoint.hpp>
#include <Core/Sandbox2D.hpp>
class SandboxApplication : public Creepy::Application {
    public:
        SandboxApplication() noexcept : Creepy::Application{"Sandbox", 600, 600}{
            std::clog << "Wow\n";
            this->PushLayer(new Sandbox2D());
        }

        virtual ~SandboxApplication() noexcept {
            
        }
    private:
};

std::unique_ptr<Creepy::Application> Creepy::CreateApplication() noexcept {
    return std::make_unique<SandboxApplication>();
}