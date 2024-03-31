#include <CreepyEngine/Scene/Scene.hpp>
#include <CreepyEngine/Renderer/Renderer2D.hpp>
#include <CreepyEngine/Scene/Entity.hpp>
#include <CreepyEngine/Scene/ScriptableEntity.hpp>
#include <CreepyEngine/Scene/Components.hpp>
#include <CreepyEngine/Scripting/ScriptEngine.hpp>

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

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

        // For sure
        this->onPhysic2DStop();

        // Remove All Exit Entity
        m_registry.clear();

        ENGINE_LOG_WARNING("A scene was destroy!!!");
    }

    Entity Scene::CreateEntity(const std::string& tag) noexcept {
        return CreateEntity(UUID{}, tag);
    }

    Entity Scene::CreateEntity(UUID uuid, const std::string& tag) noexcept {
        Entity entity{m_registry.create(), this};
        
        entity.AddComponent<IDComponent>(uuid);
        entity.AddComponent<TagComponent>(tag);
        entity.AddComponent<TransformComponent>();

        return entity;
    }

    void Scene::DestroyEntity(Entity& entity) noexcept {
        APP_LOG_WARNING("Destroy ID: {}", static_cast<uint32_t>(entity));
        m_registry.destroy(entity.m_entityHandle);
        entity.m_scene = nullptr;
    }

    void Scene::OnUpdateEditor(TimeStep timeStep, EditorCamera& camera) noexcept {
        this->renderScene(camera);
    }

    void Scene::OnUpdateRunTime(TimeStep timeStep) noexcept {

        // Native Script
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

        // Script
        {   
            auto&& scriptEntity = m_registry.view<ScriptComponent>();

            for(auto&& entityID : scriptEntity){
                Entity entity{entityID, this};
                ScriptEngine::OnUpdateEntity(entity, timeStep.GetSeconds());
            }
        }

        // Physic

        {
            constexpr const int32_t velocityIteration = 8;
            constexpr const int32_t positionIteration = 3;

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

                {
                    m_registry.view<TransformComponent, SpriteComponent>().each([](auto entityID, TransformComponent& transformComponent, SpriteComponent& spriteComponent){

                        Renderer2D::DrawSprite(transformComponent, spriteComponent, static_cast<uint32_t>(entityID));

                    });
                }

                {
                    m_registry.view<TransformComponent, CircleSpriteComponent>().each([](auto entityID, TransformComponent& transformComponent, CircleSpriteComponent& circle){
                        Renderer2D::DrawCircle(transformComponent, circle, static_cast<uint32_t>(entityID));
                    });
                }

                Renderer2D::EndScene();
            }
        }

    }

    void Scene::OnUpdateSimulation(TimeStep timeStep, EditorCamera& camera) noexcept {
        // Physic

        {
            constexpr const int32_t velocityIteration{8};
            constexpr const int32_t positionIteration{3};

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
        this->renderScene(camera);
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
        this->onPhysic2DStart();
        
        {

            ScriptEngine::OnRunTimeStart(this);

            auto&& scriptEntity = m_registry.view<ScriptComponent>();
            for(auto&& entityID : scriptEntity){
                Entity entity{entityID, this};
                ScriptEngine::OnCreateEntity(entity);
            }

        }
    }

    void Scene::OnRuntimeStop() noexcept {
        this->onPhysic2DStop();
        
        ScriptEngine::OnRunTimeStop();
    }

    void Scene::OnSimulationPlay() noexcept {
        this->onPhysic2DStart();
    }

    void Scene::OnSimulationStop() noexcept {
        this->onPhysic2DStop();
    }

    void Scene::onPhysic2DStart() noexcept {
        m_physicWorld = new b2World(b2Vec2{0.0f, -9.8f});

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
                
                boxShape.SetAsBox(transform.Scale.x * bxCol2D.Size.x, transform.Scale.y * bxCol2D.Size.y, {bxCol2D.Offset.x, bxCol2D.Offset.y}, 0.0f);
                
                b2FixtureDef fixtureDef;
                fixtureDef.shape = &boxShape;
                fixtureDef.density = bxCol2D.Density;
                fixtureDef.friction = bxCol2D.Friction;
                fixtureDef.restitution = bxCol2D.Restitution;
                fixtureDef.restitutionThreshold = bxCol2D.RestitutionThreshold;

                body->CreateFixture(&fixtureDef);
            }

            if(entity.HasComponent<CircleCollider2DComponent>()){

                auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

                b2CircleShape circleShape;
                circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
                circleShape.m_radius = cc2d.Radius * transform.Scale.x;

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &circleShape;
                fixtureDef.density = cc2d.Density;
                fixtureDef.friction = cc2d.Friction;
                fixtureDef.restitution = cc2d.Restitution;
                fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;

                body->CreateFixture(&fixtureDef);
            }

        }
    }

    void Scene::onPhysic2DStop() noexcept {
        if(!m_physicWorld){
            return;
        }

        auto&& entityDestroy = m_registry.view<RigidBody2DComponent>();

        for(auto&& entityID : entityDestroy){
            Entity entity{entityID, this};
            m_physicWorld->DestroyBody(static_cast<b2Body*>(entity.GetComponent<RigidBody2DComponent>().RuntimeBody));

            entity.GetComponent<RigidBody2DComponent>().RuntimeBody = nullptr;
        }

        delete m_physicWorld;
        m_physicWorld = nullptr;
    }

    void Scene::renderScene(EditorCamera& editorCamera) noexcept {

        Renderer2D::BeginScene(editorCamera);

        {
            m_registry.view<TransformComponent, SpriteComponent>().each([](auto entityID, TransformComponent& transformComponent, SpriteComponent& spriteComponent){

                Renderer2D::DrawSprite(transformComponent, spriteComponent, static_cast<uint32_t>(entityID));
            
            });

        }

        {
            m_registry.view<TransformComponent, CircleSpriteComponent>().each([](auto entityID, TransformComponent& transformComponent, CircleSpriteComponent& circle){
                Renderer2D::DrawCircle(transformComponent, circle, static_cast<uint32_t>(entityID));
            });
        }


        Renderer2D::EndScene();
    }

    template <typename Component>
    static void CopyComponentIfExits(Entity& srcEntity, Entity& destEntity) noexcept {
        if(srcEntity.HasComponent<Component>()){
            destEntity.AddOrReplaceComponent<Component>(srcEntity.GetComponent<Component>());
        }
    }

    template <typename... Components>
    struct CopyAllComponents{

        static void CopyComponents(const entt::registry& sources, entt::registry& dest, const std::unordered_map<UUID, entt::entity>& enttMap) noexcept;

        static void CopyComponentIfExits(Entity& srcEntity, Entity& destEntity) noexcept;

    };

    template<>
    struct CopyAllComponents<>{

        static void CopyComponents(const entt::registry& sources, entt::registry& dest, const std::unordered_map<UUID, entt::entity>& enttMap) noexcept {}

        static void CopyComponentIfExits(Entity& srcEntity, Entity& destEntity) noexcept { }

    };

    template <typename Com1, typename... Comps>
    struct CopyAllComponents<Com1, Comps...>{

        static void CopyComponents(const entt::registry& sources, entt::registry& dest, const std::unordered_map<UUID, entt::entity>& enttMap) noexcept{
            auto&& componentList = sources.view<Com1>();

            for(auto&& sourcesEntity : componentList){

                const UUID uuid = sources.get<IDComponent>(sourcesEntity).ID;

                auto destEntity = enttMap.at(uuid);

                dest.emplace_or_replace<Com1>(destEntity, sources.get<Com1>(sourcesEntity));

            }
            CopyAllComponents<Comps...>::CopyComponents(sources, dest, enttMap);
        }


        static void CopyComponentIfExits(Entity& srcEntity, Entity& destEntity) noexcept {

            if(srcEntity.HasComponent<Com1>()){
                destEntity.AddOrReplaceComponent<Com1>(srcEntity.GetComponent<Com1>());
            }

            CopyAllComponents<Comps...>::CopyComponentIfExits(srcEntity, destEntity);
        }
    };

    Ref<Scene> Scene::Copy(const Ref<Scene>& scene) noexcept {
        Ref<Scene> newScene = std::make_shared<Scene>();

        newScene->m_viewPortWidth = scene->m_viewPortWidth;
        newScene->m_viewPortHeight = scene->m_viewPortHeight;

        std::unordered_map<UUID, entt::entity> enttMap;

        auto& srcRegistry = scene->m_registry;
        auto& destRegistry = newScene->m_registry;

        auto&& entities = srcRegistry.view<IDComponent>();

        for(auto& sourcesEntity : entities){

            auto uuid = srcRegistry.get<IDComponent>(sourcesEntity).ID;
            const auto& name = srcRegistry.get<TagComponent>(sourcesEntity).Tag;

            auto entity = newScene->CreateEntity(uuid, name);
            enttMap[uuid] = entity.m_entityHandle;
        }

        // Copy Component except IDComponent, TagComponent

        CopyAllComponents<TransformComponent, SpriteComponent, CircleSpriteComponent, CameraComponent, NativeScriptComponent,
         ScriptComponent, RigidBody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent>::CopyComponents(srcRegistry, destRegistry, enttMap);

        return newScene;
    }

    void Scene::DuplicateEntity(Entity& entity) noexcept {

        Entity newEntity = CreateEntity(entity.GetName());

        CopyAllComponents<TransformComponent, SpriteComponent, CircleSpriteComponent, CameraComponent, NativeScriptComponent
        , ScriptComponent, RigidBody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent>::CopyComponentIfExits(entity, newEntity);

    }
}