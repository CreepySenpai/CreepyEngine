#include <CreepyEngine/Project/ProjectSerializer.hpp>
#include <CreepyEngine/Project/Project.hpp>
#include <CreepyEngine/Core/Core.hpp>
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
            writer << YAML::Key << "StartScene" << YAML::Value << config.StartScene.string();
            writer << YAML::Key << "AssetDirectory" << YAML::Value << config.AssetDirectoryPath.string();
            writer << YAML::Key << "ScriptPath" << YAML::Value << config.ScriptPath.string();
            writer << YAML::Key << "MeshPath" << YAML::Value << config.MeshPath.string();

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
        std::ifstream fileIn{filePath};
        std::stringstream stream;
        stream << fileIn.rdbuf();
        YAML::Node projectData = YAML::Load(stream.str());
        
        auto&& projectNode = projectData["Project"];
        
        if(!projectNode){
            return false;
        }

        auto&& config = m_project->GetConfig();

        auto&& nameNode = projectNode["Name"];
        
        auto&& startScene = projectNode["StartScene"];

        auto&& assetDir = projectNode["AssetDirectory"];
        
        auto&& scriptDir = projectNode["ScriptPath"];
        
        auto&& meshDir = projectNode["MeshPath"];

        
        config.ProjectName = nameNode.as<std::string>();
        
        config.StartScene = std::filesystem::path{startScene.as<std::string>()};
        
        config.AssetDirectoryPath = std::filesystem::path{assetDir.as<std::string>()};
        
        config.ScriptPath = std::filesystem::path{scriptDir.as<std::string>()};
        
        config.MeshPath = std::filesystem::path{meshDir.as<std::string>()};
        
        ENGINE_LOG_WARNING("Path: {}", config.StartScene.string());

        fileIn.close();
        return true;
    }

}