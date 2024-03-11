#pragma once

#include <glm/glm.hpp>
#include <string>
#include <functional>

#include <CreepyEngine/Core/TimeStep.hpp>
#include "SceneCamera.hpp"
#include "ScriptableEntity.hpp"

namespace Creepy {

    struct TagComponent{
        std::string Tag{""};

        constexpr TagComponent() noexcept = default;
        
        constexpr TagComponent(const std::string& tag) noexcept : Tag{tag} {}
    };

    struct TransformComponent
    {
        glm::mat4 Transform{1.0f};

        constexpr TransformComponent() noexcept = default;

        constexpr TransformComponent(const glm::mat4& transform) noexcept : Transform{transform} {}

    };

    struct SpriteComponent {

        glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};

        constexpr SpriteComponent() noexcept = default;

        constexpr SpriteComponent(const glm::vec4& color) noexcept : Color{color} {}

    };
    
    struct CameraComponent {
        SceneCamera Camera;
        bool IsPrimary{true};
        bool FixedAspectRatio{false};

        CameraComponent() noexcept = default;
    };

    // TODO: Change to new file
    template <typename T>
    concept IsScriptableEntity = std::derived_from<T, ScriptableEntity>;

    struct NativeScriptComponent {
        ScriptableEntity* Instance{nullptr};

        std::function<ScriptableEntity*()> CreateScript;

        std::function<void(NativeScriptComponent*)> DestroyScript;

        template <IsScriptableEntity T>
        void Bind() noexcept {

            CreateScript = [](){
                return static_cast<ScriptableEntity*>(new T);
            };

            DestroyScript = [](NativeScriptComponent* script){
                delete script->Instance;
                script->Instance = nullptr;
            };

        }
    };

}