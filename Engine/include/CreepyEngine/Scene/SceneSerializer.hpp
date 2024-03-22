#pragma once

#include <filesystem>

#include <CreepyEngine/Core/Core.hpp>
#include "Scene.hpp"

namespace Creepy {

    class SceneSerializer
    {
        public:
            SceneSerializer(const Ref<Scene>& scene) noexcept;
            
            void SerializeToYaml(const std::filesystem::path& filePath) noexcept;
            void SerializeToBinary(const std::filesystem::path& filePath) noexcept;

            [[nodiscard]] bool DeserializeFromYaml(const std::filesystem::path& filePath) noexcept;
            [[nodiscard]] bool DeserializeFromBinary(const std::filesystem::path& filePath) noexcept;
        private:
            Ref<Scene> m_scene;
    };
}