#pragma once

#include <string>
#include <filesystem>

#include <CreepyEngine/Core/Core.hpp>

namespace Creepy {

    struct ProjectConfig{
        std::string ProjectName{"Untitled"};
        std::filesystem::path StartScene;
        std::filesystem::path AssetDirectoryPath;
        std::filesystem::path ScriptPath;
        std::filesystem::path MeshPath;
    };

    class Project
    {
        public:
            // Project() noexcept;

            static inline const std::filesystem::path& GetAssetDirectory() noexcept {
                return s_activeProject->m_config.AssetDirectoryPath;
            }

            static inline constexpr ProjectConfig& GetConfig() noexcept {
                return s_activeProject->m_config;
            }

            static Ref<Project> Load(const std::filesystem::path& projectPath) noexcept;
        private:
            ProjectConfig m_config;

            inline static Ref<Project> s_activeProject;
    };
}