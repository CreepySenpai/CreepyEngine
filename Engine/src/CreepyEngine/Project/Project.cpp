#include <CreepyEngine/Project/Project.hpp>
#include <CreepyEngine/Project/ProjectSerializer.hpp>
#include <CreepyEngine/Core/Core.hpp>


namespace Creepy {

    Ref<Project> Project::s_activeProject = nullptr;

    Ref<Project> Project::New() noexcept {
        s_activeProject = std::make_shared<Project>();
        
        return s_activeProject;
    }

    Ref<Project> Project::Load(const std::filesystem::path& projectPath) noexcept {
        auto project = std::make_shared<Project>();

        ProjectSerializer serializer{project};

        if(serializer.DeserializeFromYaml(projectPath)) {
            s_activeProject = project;
            return s_activeProject;
        }
        
        return nullptr;
    }

    bool Project::Save(const std::filesystem::path& filetPath) noexcept {
        ProjectSerializer serializer{s_activeProject};
        return serializer.SerializeToYaml(filetPath);
    }
}