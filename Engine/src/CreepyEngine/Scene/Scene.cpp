#include <CreepyEngine/Scene/Scene.hpp>
#include <CreepyEngine/Renderer/Renderer2D.hpp>
#include <CreepyEngine/Scene/Entity.hpp>
// #include <CreepyEngine/Scene/Components.hpp>

namespace Creepy {

    Scene::Scene() noexcept {

    }

    Scene::~Scene() noexcept {
         
    }

    [[nodiscard]] Entity Scene::CreateEntity(const std::string& tag) noexcept {
        Entity entity{m_registry.create(), this};

        // By default all entity will have transform, tag component
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<TagComponent>(tag);
        return entity;
    }

    void Scene::OnUpdate(const TimeStep& timeStep) noexcept {

        auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);

        for(auto entity : group){
            
            auto&& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

            Renderer2D::DrawRect(transform.Transform, sprite.Color);
        }

    }


}