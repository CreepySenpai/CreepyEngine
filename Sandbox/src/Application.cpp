#include <iostream>
#include <CreepyEngine.hpp>
#include <CreepyEngine/Core/EntryPoint.hpp>
#include <Core/Sandbox2D.hpp>
class SandboxApplication : public Creepy::Application {
    public:
        SandboxApplication() noexcept{
            std::clog << "Wow\n";
            this->PushLayer(new Sandbox2D());
        }

        virtual ~SandboxApplication() noexcept {
            
        }
    private:
};

std::unique_ptr<Creepy::Application> Creepy::CreateApplication() {
    return std::make_unique<SandboxApplication>();
}