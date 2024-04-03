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

    inline static std::unordered_map<std::string_view, ScriptFieldDataType> s_scriptFieldDataMap{
        {"System.Boolean", ScriptFieldDataType::BOOL},
        {"System.Byte", ScriptFieldDataType::BYTE},
        {"System.SByte", ScriptFieldDataType::SBYTE},
        {"System.Char", ScriptFieldDataType::CHAR},
        {"System.Int16", ScriptFieldDataType::SHORT},
        {"System.UInt16", ScriptFieldDataType::USHORT},
        {"System.Int32", ScriptFieldDataType::INT},
        {"System.UInt32", ScriptFieldDataType::UINT},
        {"System.IntPtr", ScriptFieldDataType::NINT},
        {"System.Int64", ScriptFieldDataType::LONG},
        {"System.UInt64", ScriptFieldDataType::ULONG},
        {"System.Single", ScriptFieldDataType::FLOAT},
        {"System.Double", ScriptFieldDataType::DOUBLE},
        {"System.Decimal", ScriptFieldDataType::DECIMAL},
        {"System.String", ScriptFieldDataType::STRING},
        {"Creepy.Entity", ScriptFieldDataType::ENTITY},
        {"System.Numerics.Vector2", ScriptFieldDataType::VECTOR2},
        {"System.Numerics.Vector3", ScriptFieldDataType::VECTOR3},
        {"System.Numerics.Vector4", ScriptFieldDataType::VECTOR4},
    };


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
        Coral::AssemblyLoadContext CoreAssemblyContext;
        Coral::ManagedAssembly CoreManagedAssembly;

        Coral::AssemblyLoadContext AppAssemblyContext;
        Coral::ManagedAssembly AppManagedAssembly;

        std::unordered_map<std::string, Coral::Type*> EntityClasses;

        std::unordered_map<UUID, Coral::ManagedObject> EntityInstances;

        Scene* SceneContext{nullptr};
    };

    static ScriptEngineData* s_scriptEngineData{nullptr};


    void ScriptEngine::Init() noexcept {

        s_scriptEngineData = new ScriptEngineData;

        initCoral();

        // LoadCoreAssembly(std::filesystem::current_path() / "ScriptCore.dll");
        // LoadAppAssembly(std::filesystem::current_path() / "SandboxProject.dll");

        // TODO: Switch to relative path
        LoadCoreAssembly("D:/WorkSpace/GameEngineProjects/CreepyEngine_Cherno/SandboxProject/Assets/Scripts/bin/Release/net8.0/ScriptCore.dll");
        LoadAppAssembly("D:/WorkSpace/GameEngineProjects/CreepyEngine_Cherno/SandboxProject/Assets/Scripts/bin/Release/net8.0/SandboxProject.dll");

        ScriptGlue::RegisterFunctions();
        ScriptGlue::RegisterComponents();
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

        s_scriptEngineData->CoreAssemblyContext = s_scriptEngineData->HostInstance.CreateAssemblyLoadContext("CoreAssemblyContext");
        s_scriptEngineData->AppAssemblyContext = s_scriptEngineData->HostInstance.CreateAssemblyLoadContext("AppAssemblyContext");

    }

    void ScriptEngine::shutDownCoral() noexcept{

        s_scriptEngineData->HostInstance.UnloadAssemblyLoadContext(s_scriptEngineData->CoreAssemblyContext);
        s_scriptEngineData->HostInstance.UnloadAssemblyLoadContext(s_scriptEngineData->AppAssemblyContext);

        s_scriptEngineData->HostInstance.Shutdown();

        Coral::GC::Collect();
    }

    // TODO: May be core have some class inherit from entity
    void ScriptEngine::LoadCoreAssembly(const std::filesystem::path& filePath) noexcept {

        s_scriptEngineData->CoreManagedAssembly = s_scriptEngineData->CoreAssemblyContext.LoadAssembly(filePath.string());

    }

    void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filePath) noexcept {

        s_scriptEngineData->EntityClasses.clear();

        s_scriptEngineData->AppManagedAssembly = s_scriptEngineData->AppAssemblyContext.LoadAssembly(filePath.string());

        // Get base class from core assembly
        Coral::Type& entityType = s_scriptEngineData->CoreManagedAssembly.GetType("Creepy.Entity");
        
        // Get deriver class from app assembly
        for(auto& type : s_scriptEngineData->AppManagedAssembly.GetTypes()){
            
            if(type->IsSubclassOf(entityType)){
                ENGINE_LOG_WARNING("Name: {}", (std::string)type->GetFullName());
                s_scriptEngineData->EntityClasses[static_cast<std::string>(type->GetFullName())] = type;

                auto fields = type->GetFields();

                for(auto& f : fields){
                    // auto t = ConvertStringToFieldType((std::string)f.GetType().GetFullName());
                    // ENGINE_LOG_WARNING("Field: {} {} {}", std::to_underlying(f.GetAccessibility()), ConvertFieldTypeToString(t), (std::string)f.GetName());
                    ENGINE_LOG_WARNING("Field: {} {} {}", std::to_underlying(f.GetAccessibility()), (std::string)f.GetType().GetFullName(), (std::string)f.GetName());
                }

            }
        }

    }
    

    Coral::ManagedAssembly& ScriptEngine::GetLoadedCoreAssembly() noexcept {
        return s_scriptEngineData->CoreManagedAssembly;
    }

    Coral::ManagedAssembly& ScriptEngine::GetLoaderAppAssembly() noexcept {
        return s_scriptEngineData->AppManagedAssembly;
    }

    bool ScriptEngine::IsClassExits(const std::string& fullClassName) noexcept {
        return s_scriptEngineData->EntityClasses.contains(fullClassName);
    }

    std::unordered_map<std::string, Coral::Type*>& ScriptEngine::GetEntityClasses() noexcept {
        return s_scriptEngineData->EntityClasses;
    }

    Coral::Type* ScriptEngine::GetEntityClass(const std::string& className) noexcept {
        if(!s_scriptEngineData->EntityClasses.contains(className)) {
            ENGINE_LOG_ERROR("Class {} doesn't exit in map");
        }
        return s_scriptEngineData->EntityClasses[className];
    }

    std::unordered_map<std::string_view, ScriptFieldDataType>& ScriptEngine::GetScriptFieldData() noexcept {
        return s_scriptFieldDataMap;
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
    
    // BUG: Make editor mode can get entity
    Coral::ManagedObject* ScriptEngine::GetEntityInstance(UUID uuid) noexcept {

        if(!s_scriptEngineData->EntityInstances.contains(uuid)){
            // ENGINE_LOG_ERROR("Entity Instance not exits");
            return nullptr;
        }

        return &s_scriptEngineData->EntityInstances[uuid];
    }
}