#include <cstdint>

#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Core/UUID.hpp>
#include <CreepyEngine/Scripting/ScriptEngine.hpp>
#include <CreepyEngine/Scripting/ScriptGlue.hpp>

#include <CreepyEngine/Scene/Scene.hpp>
#include <CreepyEngine/Scene/Entity.hpp>

#include <Coral/HostInstance.hpp>
#include <Coral/GC.hpp>
#include <Coral/Array.hpp>
#include <Coral/Attribute.hpp>


namespace Creepy {

    // TODO: Move to utils
    static void messageCallBack(std::string_view mess, Coral::MessageLevel level) noexcept {

        switch(level){
            case Coral::MessageLevel::Info:
                ENGINE_LOG_INFO("C# call: {}",mess);
                break;
            case Coral::MessageLevel::Warning:
                ENGINE_LOG_WARNING("C# call: {}",mess);
                break;
            case Coral::MessageLevel::Error:
                ENGINE_LOG_ERROR("C# call: {}", mess);
                break;
            case Coral::MessageLevel::All:
                ENGINE_LOG_WARNING("C# call super info: {}", mess);
                break;
        }

    }

    static void exceptionCallback(std::string_view message) noexcept {
        ENGINE_LOG_ERROR("Unhandled Exception: {}!!!", message);
    }

    struct ObjectHold{
        Coral::ManagedObject* instance{nullptr};

        ObjectHold(Coral::ManagedObject* obj) : instance{obj}{

        }
    };


    struct ScriptEngineData {

        Coral::HostSettings HostSetting;
        Coral::HostInstance HostInstance;
        Coral::AssemblyLoadContext AssemblyContext;
        Coral::ManagedAssembly ManagedAssembly;

        std::unordered_map<std::string, Coral::Type*> EntityClasses;

        std::unordered_map<UUID, Coral::ManagedObject> EntityInstances;

        Scene* SceneContext{nullptr};
    };

    static ScriptEngineData* s_scriptEngineData{nullptr};


    void ScriptEngine::Init() noexcept {

        s_scriptEngineData = new ScriptEngineData;

        initCoral();

        LoadAssembly(std::filesystem::current_path() / "ScriptCore.dll");

        ScriptGlue::RegisterFunctions();
    }

    void ScriptEngine::ShutDown() noexcept {

        shutDownCoral();

        delete s_scriptEngineData;
        s_scriptEngineData = nullptr;

        ENGINE_LOG_WARNING("Shut Down Script Engine");
    }

    void ScriptEngine::initCoral() noexcept {

        auto currentPath = std::filesystem::current_path();
        s_scriptEngineData->HostSetting.CoralDirectory = currentPath.string();
        s_scriptEngineData->HostSetting.MessageCallback = &messageCallBack;
        s_scriptEngineData->HostSetting.ExceptionCallback = &exceptionCallback;

        auto success = s_scriptEngineData->HostInstance.Initialize(s_scriptEngineData->HostSetting);

        ENGINE_ASSERT(success, "Init Scripting Engine");

        s_scriptEngineData->AssemblyContext = s_scriptEngineData->HostInstance.CreateAssemblyLoadContext("MyTextContext");

    }

    void ScriptEngine::shutDownCoral() noexcept{

        s_scriptEngineData->HostInstance.UnloadAssemblyLoadContext(s_scriptEngineData->AssemblyContext);

        Coral::GC::Collect();
    }

    void ScriptEngine::LoadAssembly(const std::filesystem::path& filePath) noexcept {
        s_scriptEngineData->EntityClasses.clear();

        s_scriptEngineData->ManagedAssembly = s_scriptEngineData->AssemblyContext.LoadAssembly(filePath.string());

        Coral::Type entityType = s_scriptEngineData->ManagedAssembly.GetType("Creepy.Entity");

        for(auto& type : s_scriptEngineData->ManagedAssembly.GetTypes()){

            if(type->IsSubclassOf(entityType)){

                s_scriptEngineData->EntityClasses[static_cast<std::string>(type->GetFullName())] = type;

            }
        }

    }
    

    Coral::ManagedAssembly& ScriptEngine::GetLoadedAssembly() noexcept {
        return s_scriptEngineData->ManagedAssembly;
    }

    bool ScriptEngine::IsClassExits(const std::string& fullClassName) noexcept {
        return s_scriptEngineData->EntityClasses.contains(fullClassName);
    }

    std::unordered_map<std::string, Coral::Type*>& ScriptEngine::GetEntityClasses() noexcept {
        return s_scriptEngineData->EntityClasses;
    }

    void ScriptEngine::OnRunTimeStart(Scene* scene) noexcept {
        s_scriptEngineData->SceneContext = scene;
    }

    void ScriptEngine::OnRunTimeStop() noexcept {
        s_scriptEngineData->SceneContext = nullptr;
        
        s_scriptEngineData->EntityInstances.clear();
    }

    void ScriptEngine::OnCreateEntity(Entity& entity) noexcept {

        auto& scriptComponent = entity.GetComponent<ScriptComponent>();

        if(ScriptEngine::IsClassExits(scriptComponent.ScriptName)){
            uint64_t uuid = entity.GetUUID().GetID();
            
            s_scriptEngineData->EntityInstances.emplace(std::make_pair(entity.GetUUID(), s_scriptEngineData->EntityClasses[scriptComponent.ScriptName]->CreateInstance(std::move(uuid))));

            s_scriptEngineData->EntityInstances[entity.GetUUID()].InvokeMethod("OnCreate");
        }

    }

    void ScriptEngine::OnUpdateEntity(Entity& entity, float timeStep) noexcept {

        s_scriptEngineData->EntityInstances[entity.GetUUID()].InvokeMethod("OnUpdate", std::move(timeStep));

    }

    Scene* ScriptEngine::GetSceneContext() noexcept {
        return s_scriptEngineData->SceneContext;
    }
    
}