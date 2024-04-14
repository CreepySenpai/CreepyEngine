#include <CreepyEngine/Core/Core.hpp>


namespace Creepy {

    class Project;

    class ProjectSerializer
    {    
        public:
            ProjectSerializer(const Ref<Project>& project) noexcept;
            
            [[nodiscard]] bool SerializeToYaml(const std::filesystem::path& filePath) noexcept;
            [[nodiscard]] bool DeserializeFromYaml(const std::filesystem::path& filePath) noexcept;

        private:
            Ref<Project> m_project;
    };

}