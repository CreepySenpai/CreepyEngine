#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Scripting/ScriptEngine.hpp>
#include <CreepyEngine/Scripting/ScriptGlue.hpp>

#include <Coral/HostInstance.hpp>
#include <Coral/GC.hpp>
#include <Coral/Array.hpp>
#include <Coral/Attribute.hpp>

#include <glm/glm.hpp>

namespace Creepy {

    #define ENGINE_ADD_INTERNAL_CALL(Func) ScriptEngine::GetLoadedAssembly().AddInternalCall("ScriptCore.InternalCalls", #Func, reinterpret_cast<void*>(&Func))
    #define PTINNAME(Func) ENGINE_LOG_WARNING("Func Name: {}", #Func)

    static void TestInternalCall(int i){
        ENGINE_LOG_WARNING("Value Print: {}", i);
    }


    static void TestPrintVec3(glm::vec3* vec){
        ENGINE_LOG_WARNING("X: {}, Y: {}, Z: {}", vec->x, vec->y, vec->z);
        vec->x = vec->x * vec->x;
        vec->y = vec->y * vec->y;
        vec->z = vec->z * vec->z;
    }

    static void TestPrintConstVec3(const glm::vec3* vec){
        ENGINE_LOG_WARNING("Const X: {}, Y: {}, Z: {}", vec->x, vec->y, vec->z);
    }

    void ScriptGlue::RegisterFunctions() noexcept {
        // // Interal Call

        ENGINE_ADD_INTERNAL_CALL(TestInternalCall);
        ENGINE_ADD_INTERNAL_CALL(TestPrintVec3);
        ENGINE_ADD_INTERNAL_CALL(TestPrintConstVec3);
        ScriptEngine::GetLoadedAssembly().UploadInternalCalls();
    }

}