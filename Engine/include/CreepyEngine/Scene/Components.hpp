#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <functional>

#include <CreepyEngine/Core/TimeStep.hpp>
#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Renderer/Texture.hpp>
#include "SceneCamera.hpp"
#include "ScriptableEntity.hpp"

namespace Creepy {

    struct TagComponent{
        std::string Tag{};

        constexpr TagComponent() noexcept = default;
        
        constexpr TagComponent(const std::string& tag) noexcept : Tag{tag} {}
    };

    struct TransformComponent
    {
        glm::vec3 Position{0.0f, 0.0f, 0.0f};
        glm::vec3 Rotation{0.0f, 0.0f, 0.0f};
        glm::vec3 Scale{1.0f, 1.0f, 1.0f};

        constexpr TransformComponent() noexcept = default;

        constexpr TransformComponent(const glm::vec3& position) noexcept : Position{position} {}

        glm::mat4 GetTransform() const noexcept {
            // Use quaternion rotation
            glm::mat4 rotation = glm::toMat4(glm::quat{Rotation});

            return glm::translate(glm::mat4{1.0f}, Position) * rotation * glm::scale(glm::mat4{1.0f}, Scale);
        }

    };

    struct SpriteComponent {

        glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
        Ref<Texture2D> Texture;
        float TilingFactor{1.0f};

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