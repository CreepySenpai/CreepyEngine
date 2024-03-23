#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Scene/SceneAddComponent.hpp>
#include <CreepyEngine/Scene/Entity.hpp>

namespace Creepy {

    // template<>
    // void Scene::OnComponentAdded<TransformComponent>(Entity& entity, TransformComponent& component) {
    //     ENGINE_LOG_WARNING("Add Transform To Entity: {}", static_cast<uint32_t>(entity));
    // }

    // template<>
    // void Scene::OnComponentAdded<SpriteComponent>(Entity& entity, SpriteComponent& component) {
    //     ENGINE_LOG_WARNING("Add Sprite To Entity: {}", static_cast<uint32_t>(entity));
    // }

    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity& entity, CameraComponent& component) {
        ENGINE_LOG_WARNING("Add Camera To Entity: {}", static_cast<uint32_t>(entity));
        component.Camera.SetViewPortSize(m_viewPortWidth, m_viewPortHeight);
    }
    

    // template<>
    // void Scene::OnComponentAdded<NativeScriptComponent>(Entity& entity, NativeScriptComponent& component) {
    //     ENGINE_LOG_WARNING("Add NativeScript To Entity: {}", static_cast<uint32_t>(entity));
    // }

    // template<> 
    // void Scene::OnComponentAdded<RigidBody2DComponent>(Entity& entity, RigidBody2DComponent& component) {
        
    // }

    // template<> 
    // void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity& entity, BoxCollider2DComponent& component) {

    // }

}