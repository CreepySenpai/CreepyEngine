#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Core/KeyCode.hpp>
#include <CreepyEngine/Core/Input.hpp>
#include <CreepyEngine/Scene/Scene.hpp>
#include <CreepyEngine/Scene/Entity.hpp>
#include <CreepyEngine/Scripting/ScriptEngine.hpp>
#include <CreepyEngine/Scripting/ScriptGlue.hpp>

#include <Coral/HostInstance.hpp>
#include <Coral/GC.hpp>
#include <Coral/Array.hpp>
#include <Coral/Attribute.hpp>

#include <glm/glm.hpp>

namespace Creepy {

    #define ENGINE_ADD_INTERNAL_CALL(Func) ScriptEngine::GetLoadedCoreAssembly().AddInternalCall("Creepy.InternalCalls", #Func, reinterpret_cast<void*>(&Func))

    static std::unordered_map<std::string, std::function<bool(Entity&)>> s_entityHasComponentFuncs;

    static bool Input_IsKeyPressed(KeyCode keyCode){
        return Input::IsKeyPressed(keyCode);
    }

    static bool Entity_HasComponent(uint64_t uuid, Coral::String componentName) {
        ENGINE_LOG_WARNING("Call Type: {}", (std::string)componentName);
        
        auto scene = ScriptEngine::GetSceneContext();

        auto&& entity = scene->GetEntity(uuid);

        return s_entityHasComponentFuncs[static_cast<std::string>(componentName)](entity);
    }

    static uint64_t Entity_GetEntityByName(Coral::String entityNativeName) {

        auto&& entityName = static_cast<std::string>(entityNativeName);

        auto scene = ScriptEngine::GetSceneContext();
        
        auto entity = scene->GetEntity(entityName);

        if(entity.IsExits()){
            return entity.GetUUID().GetID();
        }
        return 0;
    }

    // TODO: Check Entity Has Script Component
    static Coral::ManagedObject Entity_GetInstance(uint64_t id){
        UUID uuid{id};
        return ScriptEngine::GetEntityInstance(uuid);
    }

    static void TransformComponent_GetPosition(uint64_t uuid , glm::vec3* outPosition){
        auto scene = ScriptEngine::GetSceneContext();

        auto&& entity = scene->GetEntity(uuid);

        *outPosition = entity.GetComponent<TransformComponent>().Position;
    }

    static void TransformComponent_SetPosition(uint64_t uuid, glm::vec3* inPosition){
        auto scene = ScriptEngine::GetSceneContext();

        auto&& entity = scene->GetEntity(uuid);

        entity.GetComponent<TransformComponent>().Position = *inPosition;
    }

    static void TransformComponent_GetRotation(uint64_t uuid, glm::vec3* outRotation) {
        auto scene = ScriptEngine::GetSceneContext();

        auto&& entity = scene->GetEntity(uuid);
        *outRotation = entity.GetComponent<TransformComponent>().Rotation;
    }

    static void TransformComponent_SetRotation(uint64_t uuid, glm::vec3* inRotation) {
        auto scene = ScriptEngine::GetSceneContext();

        auto&& entity = scene->GetEntity(uuid);
        entity.GetComponent<TransformComponent>().Rotation = *inRotation;
    }

    static void TransformComponent_GetScale(uint64_t uuid , glm::vec3* outScale){
        auto scene = ScriptEngine::GetSceneContext();

        auto&& entity = scene->GetEntity(uuid);

        *outScale = entity.GetComponent<TransformComponent>().Scale;
    }

    static void TransformComponent_SetScale(uint64_t uuid, glm::vec3* inScale){
        auto scene = ScriptEngine::GetSceneContext();

        auto&& entity = scene->GetEntity(uuid);

        entity.GetComponent<TransformComponent>().Scale = *inScale;
    }

    void ScriptGlue::RegisterFunctions() noexcept {
        // // Interal Call
        ENGINE_ADD_INTERNAL_CALL(Input_IsKeyPressed);
        ENGINE_ADD_INTERNAL_CALL(Entity_HasComponent);
        ENGINE_ADD_INTERNAL_CALL(Entity_GetEntityByName);
        ENGINE_ADD_INTERNAL_CALL(Entity_GetInstance);
        ENGINE_ADD_INTERNAL_CALL(TransformComponent_GetPosition);
        ENGINE_ADD_INTERNAL_CALL(TransformComponent_SetPosition);
        ENGINE_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
        ENGINE_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
        ENGINE_ADD_INTERNAL_CALL(TransformComponent_GetScale);
        ENGINE_ADD_INTERNAL_CALL(TransformComponent_SetScale);
       
        ScriptEngine::GetLoadedCoreAssembly().UploadInternalCalls();
    }

    // TODO: Use reflection to get name, use variadic template
    template <typename T>
    static void RegisterComponent() noexcept {

        std::string typeName{typeid(T).name()};
        typeName.erase(0, 10);
        typeName.pop_back();
        std::string managedTypeName{std::format("Creepy.{}", typeName)};
        ENGINE_LOG_WARNING("Name: {}", managedTypeName);

        s_entityHasComponentFuncs.emplace(std::make_pair(managedTypeName, [](Entity& entity){
            return entity.HasComponent<T>();
        }));
    }

    // template <typename... Components>
    // struct Register{
    //     constexpr static void RegisterComponentss() noexcept;
    // };

    // template <>
    // struct Register<>{
    //     constexpr static void RegisterComponentss() noexcept;
    // };

    // template <typename Comp1, typename... Components>
    // struct Register<Comp1, Components...>{

    //     constexpr static void RegisterComponentss() noexcept {

    //         std::string typeName{typeid(Comp1).name()};
    //         typeName.erase(0, 10);
    //         typeName.pop_back();
    //         std::string managedTypeName{std::format("Creepy.{}", typeName)};
    //         ENGINE_LOG_WARNING("Name: {}", managedTypeName);

    //         s_entityHasComponentFuncs.emplace(std::make_pair(managedTypeName, [](Entity& entity){
    //             return entity.HasComponent<Comp1>();
    //         }));

    //         Register<Components...>::RegisterComponentss();
    //     }
    // };

    void ScriptGlue::RegisterComponents() noexcept {
        s_entityHasComponentFuncs.clear();
        
        RegisterComponent<IDComponent>();
        RegisterComponent<TagComponent>();
        RegisterComponent<TransformComponent>();
        RegisterComponent<SpriteComponent>();
        RegisterComponent<CircleSpriteComponent>();
        RegisterComponent<CameraComponent>();
        RegisterComponent<NativeScriptComponent>();
        RegisterComponent<ScriptComponent>();
        RegisterComponent<RigidBody2DComponent>();
        RegisterComponent<BoxCollider2DComponent>();
        RegisterComponent<CircleCollider2DComponent>();
        RegisterComponent<MeshComponent>();
        RegisterComponent<RigidBody3DComponent>();

        ENGINE_LOG_WARNING("Component Regis: {}", s_entityHasComponentFuncs.size());
    }

}