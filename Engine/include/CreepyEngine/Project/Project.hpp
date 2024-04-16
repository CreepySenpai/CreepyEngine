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

    class [[nodiscard]] Project
    {
        public:

            ~Project() noexcept{
                ENGINE_LOG_WARNING("Destroyyyyyyyyyyyyyy");
            }

            inline constexpr const std::filesystem::path& GetAssetDirectory() noexcept {
                return s_activeProject->m_config.AssetDirectoryPath;
            }

            inline constexpr ProjectConfig& GetConfig() noexcept {
                return m_config;
            }

            static Ref<Project> GetActive() noexcept {
                return s_activeProject;
            }

            static Ref<Project> New() noexcept;
            static Ref<Project> Load(const std::filesystem::path& projectPath) noexcept;
            static bool Save(const std::filesystem::path& filetPath) noexcept;

        private:
            ProjectConfig m_config;

            static Ref<Project> s_activeProject;
    };
}