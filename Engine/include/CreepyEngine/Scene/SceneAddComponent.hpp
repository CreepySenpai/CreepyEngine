#pragma once

#include "Scene.hpp"
#include "Components.hpp"

namespace Creepy{
    template<> void Scene::OnComponentAdded<TransformComponent>(Entity& entity, TransformComponent& component);

    template<> void Scene::OnComponentAdded<CameraComponent>(Entity& entity, CameraComponent& component);

    template<> void Scene::OnComponentAdded<SpriteComponent>(Entity& entity, SpriteComponent& component);

    template<> void Scene::OnComponentAdded<NativeScriptComponent>(Entity& entity, NativeScriptComponent& component);
}