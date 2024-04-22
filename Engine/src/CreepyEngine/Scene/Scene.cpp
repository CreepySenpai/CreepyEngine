#include <CreepyEngine/Scene/Scene.hpp>
#include <CreepyEngine/Core/TimeStep.hpp>
#include <CreepyEngine/Renderer/EditorCamera.hpp>
#include <CreepyEngine/Renderer/Renderer.hpp>
#include <CreepyEngine/Scene/Entity.hpp>
#include <CreepyEngine/Scene/ScriptableEntity.hpp>
#include <CreepyEngine/Scene/Components.hpp>
#include <CreepyEngine/Scripting/ScriptEngine.hpp>

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

#include <CreepyEngine/Physics/2D/PhysicWorld2D.hpp>

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
        
        m_entityMap[uuid] = entity.m_entityHandle;

        return entity;
    }


    Entity Scene::GetEntity(UUID uuid) noexcept {

        if(m_entityMap.contains(uuid)){
            return {m_entityMap.at(uuid), this};
        }

        return {};
    }

    Entity Scene::GetEntity(std::string_view entityName) noexcept {

        auto views = m_registry.view<TagComponent>();

        for(auto&& entityID : views){

            Entity entity{entityID, this};
            
            if(entity.GetName() == entityName){
                return entity;
            }
        }

        return {};
    }

    void Scene::DestroyEntity(Entity& entity) noexcept {
        APP_LOG_WARNING("Destroy ID: {}", static_cast<uint32_t>(entity));
        m_entityMap.erase(entity.GetUUID());
        m_registry.destroy(entity.m_entityHandle);
        entity.m_scene = nullptr;
    }

    void Scene::OnUpdateEditor(TimeStep timeStep, EditorCamera& camera) noexcept {
        this->renderScene(camera);
    }

    void Scene::OnUpdateRunTime(TimeStep timeStep) noexcept {

        if(!m_isScenePause || m_stepFrame-- > 0){
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
                this->onScriptUpdate(timeStep);
            }

            // Physic
            {
                this->onPhysicUpdate(timeStep);
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

                Renderer::BeginScene(*mainCamera, transformMatrix);

                this->onRenderUpdate();

                Renderer::EndScene();
            }
        }

    }

    void Scene::OnUpdateSimulation(TimeStep timeStep, EditorCamera& camera) noexcept {
        // Physic
        if(!m_isScenePause || m_stepFrame-- > 0)
        {
            this->onPhysicUpdate(timeStep);
        }
        

        // Render
        this->renderScene(camera);
    }


    void Scene::OnViewPortResize(uint32_t width, uint32_t height) noexcept {

        if(m_viewPortWidth == width && m_viewPortHeight == height) {
            return;
        }

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
            auto&& camera = entitiesHasCamera.get<CameraComponent>(entity);
            
            if(camera.IsPrimary){
                return Entity{entity, this};
            }
       }

       return {};
    }

    void Scene::OnRuntimePlay() noexcept {
        m_isScenePlay = true;
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
        m_isScenePlay = false;
        this->onPhysic2DStop();
        
        ScriptEngine::OnRunTimeStop();
    }

    void Scene::OnSimulationPlay() noexcept {
        this->onPhysic2DStart();
    }

    void Scene::OnSimulationStop() noexcept {
        this->onPhysic2DStop();
    }

    bool Scene::IsScenePlay() const noexcept {
        return m_isScenePlay;
    }

    bool Scene::IsScenePause() const noexcept {
        return m_isScenePause;
    }

    void Scene::Step(int frame) noexcept {
        m_stepFrame = frame;
    }

    void Scene::onPhysic2DStart() noexcept {
        
        Physic2D::PhysicWorld2D::InitWorld2D(this);

    }

    void Scene::onPhysic2DStop() noexcept {

        Physic2D::PhysicWorld2D::DestroyWorld2D(this);
    }

    void Scene::renderScene(EditorCamera& editorCamera) noexcept {

        Renderer::BeginScene(editorCamera);

        this->onRenderUpdate();

        Renderer::EndScene();
    }

    void Scene::onScriptUpdate(TimeStep timeStep) noexcept {
        auto&& scriptEntity = m_registry.view<ScriptComponent>();

        for (auto&& entityID : scriptEntity)
        {
            Entity entity{entityID, this};
            ScriptEngine::OnUpdateEntity(entity, timeStep.GetSeconds());
        }
    }

    void Scene::onPhysicUpdate(TimeStep timeStep) noexcept {
        
        Physic2D::PhysicWorld2D::UpdateWorld2D(timeStep, this);
    }

    void Scene::onRenderUpdate() noexcept {
        {
            m_registry.view<TransformComponent, SpriteComponent>().each([](auto entityID, TransformComponent& transformComponent, SpriteComponent& spriteComponent){

                Renderer::DrawSprite(transformComponent, spriteComponent, static_cast<uint32_t>(entityID));
            
            });

        }

        {
            m_registry.view<TransformComponent, CircleSpriteComponent>().each([](auto entityID, TransformComponent& transformComponent, CircleSpriteComponent& circle){
                Renderer::DrawCircle(transformComponent, circle, static_cast<uint32_t>(entityID));
            });
        }

        {
            m_registry.view<TransformComponent, MeshComponent>().each([](auto entityID, TransformComponent& transformComponent, MeshComponent& meshComponent){
                switch(meshComponent.Type){
                    case MeshComponent::MeshType::CUBE: {
                        Renderer::DrawCube(transformComponent.GetTransform(), {160.0f / 255.0f, 163.0f / 255.0f, 168.0f / 255.0f, 1.0f}, static_cast<int>(entityID));
                        break;
                    }

                    case MeshComponent::MeshType::CYLINDER: {
                        Renderer::DrawCylinder(transformComponent.GetTransform(), {160.0f / 255.0f, 163.0f / 255.0f, 168.0f / 255.0f, 1.0f}, static_cast<int>(entityID));
                        break;
                    }

                    case MeshComponent::MeshType::CONE: {
                        Renderer::DrawCone(transformComponent.GetTransform(), {160.0f / 255.0f, 163.0f / 255.0f, 168.0f / 255.0f, 1.0f}, static_cast<int>(entityID));
                        break;
                    }

                    case MeshComponent::MeshType::TORUS: {
                        Renderer::DrawTorus(transformComponent.GetTransform(), {160.0f / 255.0f, 163.0f / 255.0f, 168.0f / 255.0f, 1.0f}, static_cast<int>(entityID));
                        break;
                    }
                }
            });
        }

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
         ScriptComponent, RigidBody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent, MeshComponent>::CopyComponents(srcRegistry, destRegistry, enttMap);

        return newScene;
    }

    void Scene::DuplicateEntity(Entity& entity) noexcept {

        Entity newEntity = CreateEntity(entity.GetName());

        CopyAllComponents<TransformComponent, SpriteComponent, CircleSpriteComponent, CameraComponent, NativeScriptComponent
        , ScriptComponent, RigidBody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent, MeshComponent, RigidBody3DComponent>::CopyComponentIfExits(entity, newEntity);

    }

    
}