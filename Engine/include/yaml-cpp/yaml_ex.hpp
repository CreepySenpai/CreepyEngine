#pragma once
#include "yaml.h"
#include <glm/glm.hpp>
#include <filesystem>

namespace YAML {

    template <>
    struct convert<glm::vec2>{
        static Node encode(const glm::vec2& vec);
        static bool decode(const Node& node, glm::vec2& vec);
    };

    template <>
    struct convert<glm::vec3>{
        static Node encode(const glm::vec3& vec);
        static bool decode(const Node& node, glm::vec3& vec);
    };

    template <>
    struct convert<glm::vec4>{
        static Node encode(const glm::vec4& vec);
        static bool decode(const Node& node, glm::vec4& vec);
    };

    Emitter& operator<<(YAML::Emitter& writer, const glm::vec2& vec);
    Emitter& operator<<(YAML::Emitter& writer, const glm::vec3& vec);
    Emitter& operator<<(YAML::Emitter& writer, const glm::vec4& vec);
    Emitter& operator<<(YAML::Emitter& writer, const std::filesystem::path& filePath);
}