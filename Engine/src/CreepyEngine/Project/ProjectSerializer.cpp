#include <CreepyEngine/Project/ProjectSerializer.hpp>
#include <CreepyEngine/Project/Project.hpp>
#include <yaml-cpp/yaml_ex.hpp>

namespace Creepy {

    ProjectSerializer::ProjectSerializer(const Ref<Project>& project) noexcept : m_project{project} {
        
    }
            
    bool ProjectSerializer::SerializeToYaml(const std::filesystem::path& filePath) noexcept {
        auto&& config = m_project->GetConfig();

        YAML::Emitter writer;

        writer << YAML::BeginMap;

        writer << YAML::Key << "Project" << YAML::Value;
        
        {
            writer << YAML::BeginMap;

            writer << YAML::Key << "Name" << YAML::Value << config.ProjectName;
            writer << YAML::Key << "StartScene" << YAML::Value << config.StartScene;
            writer << YAML::Key << "AssetDirectory" << YAML::Value << config.AssetDirectoryPath;
            writer << YAML::Key << "ScriptPath" << YAML::Value << config.ScriptPath;
            writer << YAML::Key << "MeshPath" << YAML::Value << config.MeshPath;

            writer << YAML::EndMap;
        }

        writer << YAML::EndMap;

        std::ofstream fileOut{filePath};
        fileOut << writer.c_str();
        fileOut.flush();
        fileOut.close();

        return true;
    }

    bool ProjectSerializer::DeserializeFromYaml(const std::filesystem::path& filePath) noexcept {

        return true;
    }

}