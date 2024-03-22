#include <CreepyEngine/Scene/Scene.hpp>
#include <CreepyEngine/Renderer/Renderer2D.hpp>
#include <CreepyEngine/Scene/Entity.hpp>
#include <CreepyEngine/Scene/Components.hpp>

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace Creepy {

    static b2BodyType ConvertRigid2DBodyTypeToBox2D(RigidBody2DComponent::BodyType bodyType) noexcept {
        switch(bodyType){
            case RigidBody2DComponent::BodyType::STATIC:
                return b2BodyType::b2_staticBody;

            case RigidBody2DComponent::BodyType::DYNAMIC:
                return b2BodyType::b2_dynamicBody;

            case RigidBody2DComponent::BodyType::KINEMATIC:
                return b2BodyType::b2_kinematicBody;
        }
        std::unreachable();
        return b2BodyType::b2_staticBody;
    }

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

        m_registry.view<TransformComponent, SpriteComponent>().each([](auto entityID, TransformComponent& transformComponent, SpriteComponent& spriteComponent){

            Renderer2D::DrawSprite(transformComponent, spriteComponent, static_cast<uint32_t>(entityID));
        
        });

        Renderer2D::EndScene();
    }

    void Scene::OnUpdateRunTime(TimeStep timeStep) noexcept {

        // Script
        {
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

        // Physic

        {
            constexpr const int32_t velocityIteration = 6;
            constexpr const int32_t positionIteration = 2;

            m_physicWorld->Step(timeStep.GetSeconds(), velocityIteration, positionIteration);

            auto&& view = m_registry.view<RigidBody2DComponent>();

            for(auto&& entityID : view){
                Entity entity{entityID, this};
                auto& transform = entity.GetComponent<TransformComponent>();
                auto& rigid2D = entity.GetComponent<RigidBody2DComponent>();

                auto body = reinterpret_cast<b2Body*>(rigid2D.RuntimeBody);
                auto&& position = body->GetPosition();
                
                transform.Position.x = position.x;
                transform.Position.y = position.y;

                transform.Rotation.z = body->GetAngle();
            }
        }

        // Render

        {
            Camera* mainCamera{nullptr};
            glm::mat4 transformMatrix;

            m_registry.view<TransformComponent,CameraComponent>().each([&mainCamera, &transformMatrix](auto entityID, TransformComponent& transformComponent ,CameraComponent& cameraComponent){

                if(cameraComponent.IsPrimary){
                    transformMatrix = transformComponent.GetTransform();
                    mainCamera = &cameraComponent.Camera;
                }

            });
            

            if(mainCamera){

                Renderer2D::BeginScene(*mainCamera, transformMatrix);

                m_registry.view<TransformComponent, SpriteComponent>().each([](auto entityID, TransformComponent& transformComponent, SpriteComponent& spriteComponent){

                    Renderer2D::DrawSprite(transformComponent, spriteComponent, static_cast<uint32_t>(entityID));

                });

                Renderer2D::EndScene();
            }
        }

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

    void Scene::OnRuntimePlay() noexcept {
        m_physicWorld = std::unique_ptr<b2World>(new b2World(b2Vec2(0.0f, -9.8f)));

        // Can not use lambda because can not capture std::unique_ptr
        auto&& rigidView  = m_registry.view<RigidBody2DComponent>();

        for(auto&& entityID : rigidView){

            Entity entity{entityID, this};
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rigid2D = entity.GetComponent<RigidBody2DComponent>();

            b2BodyDef bodyDef;

            bodyDef.type = ConvertRigid2DBodyTypeToBox2D(rigid2D.Type);

            bodyDef.position.Set(transform.Position.x, transform.Position.y);
            bodyDef.angle = transform.Rotation.z;

            auto body = m_physicWorld->CreateBody(&bodyDef);

            body->SetFixedRotation(rigid2D.FixedRotation);

            rigid2D.RuntimeBody = body;

            if(entity.HasComponent<BoxCollider2DComponent>()){
                auto& bxCol2D = entity.GetComponent<BoxCollider2DComponent>();
                
                b2PolygonShape boxShape;
                boxShape.SetAsBox(transform.Scale.x * bxCol2D.Size.x, transform.Scale.y * bxCol2D.Size.y);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &boxShape;
                fixtureDef.density = bxCol2D.Density;
                fixtureDef.friction = bxCol2D.Friction;
                fixtureDef.restitution = bxCol2D.Restitution;
                fixtureDef.restitutionThreshold = bxCol2D.RestitutionThreshold;

                body->CreateFixture(&fixtureDef);
            }

            
        }
    }

    void Scene::OnRuntimeStop() noexcept {
        auto&& entityDestroy = m_registry.view<RigidBody2DComponent>();

        for(auto&& entityID : entityDestroy){
            Entity entity{entityID, this};
            m_physicWorld->DestroyBody(reinterpret_cast<b2Body*>(entity.GetComponent<RigidBody2DComponent>().RuntimeBody));
        }

        m_physicWorld.reset();
    }
}