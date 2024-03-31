#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Scripting/ScriptEngine.hpp>
#include <CreepyEngine/Scripting/ScriptGlue.hpp>

#include <Coral/HostInstance.hpp>
#include <Coral/GC.hpp>
#include <Coral/Array.hpp>
#include <Coral/Attribute.hpp>

#include <glm/glm.hpp>

namespace Creepy {

    #define ENGINE_ADD_INTERNAL_CALL(Func) ScriptEngine::GetLoadedAssembly().AddInternalCall("Creepy.InternalCalls", #Func, reinterpret_cast<void*>(&Func))
    #define PTINNAME(Func) ENGINE_LOG_WARNING("Func Name: {}", #Func)

    static void Entity_GetPosition(uint64_t uuid , glm::vec3* inPosition){

    }

    static void Entity_SetPosition(uint64_t uuid, glm::vec3* outPosition){
        // auto scene = ScriptEngine::GetSceneContext();


    }

    void ScriptGlue::RegisterFunctions() noexcept {
        // // Interal Call
        ENGINE_ADD_INTERNAL_CALL(Entity_GetPosition);
        ENGINE_ADD_INTERNAL_CALL(Entity_SetPosition);
       
        ScriptEngine::GetLoadedAssembly().UploadInternalCalls();
    }

}