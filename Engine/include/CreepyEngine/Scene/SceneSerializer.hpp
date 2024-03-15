#pragma once

#include <CreepyEngine/Core/Core.hpp>
#include "Scene.hpp"

namespace Creepy {

    class SceneSerializer
    {
        public:
            SceneSerializer(const Ref<Scene>& scene) noexcept;
            
            void SerializeToYaml(const std::string& filePath) noexcept;
            void SerializeToBinary(const std::string& filePath) noexcept;

            bool DeserializeFromYaml(const std::string& filePath) noexcept;
            bool DeserializeFromBinary(const std::string& filePath) noexcept;
        private:
            Ref<Scene> m_scene;
    };
}