#include <CreepyEngine.hpp>
#include <CreepyEngine/Core/EntryPoint.hpp>
#include <Editor/EditorLayer.hpp>

namespace Creepy{

    class CreepyEditor : public Application
    {
        public:
            CreepyEditor() noexcept : Application{"CreepyEditor", 1600, 900} {

                // TODO: Move to another place
                Project::Load("./Sandbox.cxproj");

                this->PushLayer(new EditorLayer());
            }

            virtual ~CreepyEditor() noexcept {
                
            }
        private:
    };
    
    Scope<Application> CreateApplication() noexcept {
        return std::make_unique<CreepyEditor>();
    }
}