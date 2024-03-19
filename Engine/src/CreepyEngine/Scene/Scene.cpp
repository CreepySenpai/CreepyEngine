#include <CreepyEngine/Scene/Scene.hpp>
#include <CreepyEngine/Renderer/Renderer2D.hpp>
#include <CreepyEngine/Scene/Entity.hpp>
#include <CreepyEngine/Scene/Components.hpp>

namespace Creepy {

    Scene::Scene() noexcept {
        
    }

    Scene::~Scene() noexcept {
        // Remove All Exit Entity
        m_registry.clear();
        ENGINE_LOG_WARNING("A scene was destroy!!!");
    }

    Entity Scene::CreateEntity(const std::string& tag) noexcept {
        Entity entity{m_registry.create(), this};

        // By default all entity will have transform, tag component
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<TagComponent>(tag);
        return entity;
    }

    void Scene::DestroyEntity(Entity& entity) noexcept {
        APP_LOG_WARNING("Destroy ID: {}", static_cast<uint32_t>(entity));
        m_registry.destroy(entity.m_entityHandle);
        entity.m_scene = nullptr;
    }

    void Scene::OnUpdateEditor(TimeStep timeStep, EditorCamera& camera) noexcept {
        auto renderEntity = m_registry.view<TransformComponent, SpriteComponent>();

        Renderer2D::BeginScene(camera);

        for (auto entity : renderEntity)
        {
            auto &&[transform, sprite] = renderEntity.get<TransformComponent, SpriteComponent>(entity);
            Renderer2D::DrawSprite(transform, sprite, static_cast<int>(entity));
        }

        Renderer2D::EndScene();
    }

    void Scene::OnUpdateRunTime(TimeStep timeStep) noexcept {

        m_registry.view<NativeScriptComponent>().each([timeStep, this](auto entity, NativeScriptComponent& nativeComponent){
            
            // TODO: Move to scene play
            if(!nativeComponent.Instance){
                nativeComponent.Instance = nativeComponent.CreateScript();
                nativeComponent.Instance->m_entity = {entity, this};
                ENGINE_LOG_WARNING("Create Native Instance!");

                nativeComponent.Instance->OnCreate();
            }

            nativeComponent.Instance->OnUpdate(timeStep);

        });
        
    }


    void Scene::OnViewPortResize(uint32_t width, uint32_t height) noexcept {
        m_viewPortWidth = width;
        m_viewPortHeight = height;

        auto entities = m_registry.view<CameraComponent>();

        for(auto& entity : entities){

            auto& cameraComp = entities.get<CameraComponent>(entity);

            if(cameraComp.FixedAspectRatio){
                ENGINE_LOG_INFO("Camera Resize {} : {}", m_viewPortWidth, m_viewPortHeight);
                cameraComp.Camera.SetViewPortSize(m_viewPortWidth, m_viewPortHeight);
            }
        }
    }

    template <typename T>
    void Scene::OnComponentAdded(Entity& entity, T& component){
        
    }

    Entity Scene::GetPrimaryCameraEntity() noexcept {
       auto&& entitiesHasCamera = m_registry.view<CameraComponent>();

       for(auto&& entity : entitiesHasCamera){
            auto camera = entitiesHasCamera.get<CameraComponent>(entity);
            
            if(camera.IsPrimary){
                return Entity{entity, this};
            }
       }

       return {};
    }
}