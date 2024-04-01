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

    #define ENGINE_ADD_INTERNAL_CALL(Func) ScriptEngine::GetLoadedAssembly().AddInternalCall("Creepy.InternalCalls", #Func, reinterpret_cast<void*>(&Func))

    static void Entity_GetPosition(uint64_t uuid , glm::vec3* outPosition){
        auto scene = ScriptEngine::GetSceneContext();

        Entity entity = scene->GetEntity(uuid);

        *outPosition = entity.GetComponent<TransformComponent>().Position;
    }

    static void Entity_SetPosition(uint64_t uuid, glm::vec3* inPosition){
        auto scene = ScriptEngine::GetSceneContext();

        auto entity = scene->GetEntity(uuid);

        entity.GetComponent<TransformComponent>().Position = *inPosition;
    }

    static bool Input_IsKeyPressed(KeyCode keyCode){
        return Input::IsKeyPressed(keyCode);
    }

    void ScriptGlue::RegisterFunctions() noexcept {
        // // Interal Call
        ENGINE_ADD_INTERNAL_CALL(Entity_GetPosition);
        ENGINE_ADD_INTERNAL_CALL(Entity_SetPosition);
        ENGINE_ADD_INTERNAL_CALL(Input_IsKeyPressed);
       
        ScriptEngine::GetLoadedAssembly().UploadInternalCalls();
    }

}