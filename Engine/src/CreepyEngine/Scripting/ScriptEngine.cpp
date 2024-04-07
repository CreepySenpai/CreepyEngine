#include <cstdint>

#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Core/UUID.hpp>
#include <CreepyEngine/Scripting/ScriptEngine.hpp>
#include <CreepyEngine/Scripting/ScriptGlue.hpp>
#include <CreepyEngine/Utils/ScriptEngineUtils.hpp>

#include <CreepyEngine/Scene/Scene.hpp>
#include <CreepyEngine/Scene/Entity.hpp>

#include <Coral/HostInstance.hpp>
#include <Coral/GC.hpp>
#include <Coral/Array.hpp>
#include <Coral/Attribute.hpp>


namespace Creepy {

    inline static std::unordered_map<std::string_view, ScriptFieldDataType> s_scriptFieldDataTypeMap{
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

    struct ScriptEngineData {

        Coral::HostSettings HostSetting;
        
        Coral::HostInstance HostInstance;
        Coral::AssemblyLoadContext CoreAssemblyContext;
        Coral::ManagedAssembly CoreManagedAssembly;

        Coral::AssemblyLoadContext AppAssemblyContext;
        Coral::ManagedAssembly AppManagedAssembly;

        std::unordered_map<std::string, Coral::Type*> EntityClasses;

        std::unordered_map<UUID, Coral::ManagedObject> EntityInstances;
        
        // Store Data For Edit Mode
        std::unordered_map<UUID, ScriptEngine::FieldMap> EntityScriptFieldData;

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

                auto className = static_cast<std::string>(type->GetFullName());

                s_scriptEngineData->EntityClasses[className] = type;
                
                //TODO: Remove
            
                for(auto&& field : type->GetFields()){
                    
                    auto dataName = static_cast<std::string>(field.GetName());

                    const auto dataType = Utils::ConvertScriptStringToFieldType(static_cast<std::string>(field.GetType().GetFullName()));
                    
                    ENGINE_LOG_WARNING("Field: {} {} {}", className, (std::string)field.GetType().GetFullName(), (std::string)field.GetName());

                    for(auto&& attbs : field.GetAttributes()){
                        ENGINE_LOG_WARNING("FieldName: {} Attb: {}", (std::string)field.GetName(), (std::string)attbs.GetType().GetFullName());
                        
                    }
                }

                // End

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

    bool ScriptEngine::IsDataTypeExits(std::string_view dataTypeName) noexcept {
        return s_scriptFieldDataTypeMap.contains(dataTypeName);
    }

    std::unordered_map<std::string, Coral::Type*>& ScriptEngine::GetEntityClasses() noexcept {
        return s_scriptEngineData->EntityClasses;
    }

    Coral::Type* ScriptEngine::GetEntityClass(const std::string& className) noexcept {
        if(!s_scriptEngineData->EntityClasses.contains(className)) {
            return nullptr;
        }
        return s_scriptEngineData->EntityClasses.at(className);
    }

    std::unordered_map<std::string_view, ScriptFieldDataType>& ScriptEngine::GetScriptFieldDataType() noexcept {
        return s_scriptFieldDataTypeMap;
    }

    std::unordered_map<UUID, ScriptEngine::FieldMap>& ScriptEngine::GetScriptFieldData() noexcept {
        return s_scriptEngineData->EntityScriptFieldData;
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
            UUID uuid = entity.GetUUID();
            uint64_t id = uuid.GetID();

            s_scriptEngineData->EntityInstances.emplace(std::make_pair(uuid, s_scriptEngineData->EntityClasses[scriptComponent.ScriptName]->CreateInstance(std::move(id))));


            // Copy Raw Data From Editor Buffer To Entity Before Call OnCreate
            if(s_scriptEngineData->EntityScriptFieldData.contains(uuid)){
                
                for(auto&& field : s_scriptEngineData->EntityClasses.at(scriptComponent.ScriptName)->GetFields()) {
                    auto&& fieldName = static_cast<std::string>(field.GetName());
                    if(fieldName == "UUID"){
                        continue;
                    }

                    if(s_scriptEngineData->EntityScriptFieldData.at(uuid).contains(fieldName)){

                        auto&& fieldMap = s_scriptEngineData->EntityScriptFieldData.at(uuid).at(fieldName);

                        s_scriptEngineData->EntityInstances.at(uuid).SetFieldValueRaw(fieldName, (void*)(fieldMap.GetValueRaw()));

                    }
                    
                }


            }

            s_scriptEngineData->EntityInstances.at(entity.GetUUID()).InvokeMethod("OnCreate");
            
        }

    }

    void ScriptEngine::OnUpdateEntity(Entity& entity, float timeStep) noexcept {

        s_scriptEngineData->EntityInstances.at(entity.GetUUID()).InvokeMethod("OnUpdate", std::move(timeStep));

    }

    Scene* ScriptEngine::GetSceneContext() noexcept {
        return s_scriptEngineData->SceneContext;
    }
    

    Coral::ManagedObject ScriptEngine::GetEntityInstance(UUID uuid) noexcept {

        if(!s_scriptEngineData->EntityInstances.contains(uuid)){
            ENGINE_LOG_ERROR("Entity Instance not exits");
        }

        return s_scriptEngineData->EntityInstances.at(uuid);
    }

    void ScriptEngine::CreateEntityFastInstanceToCopyData(UUID uuid, Coral::Type* type) noexcept {

        auto entityInstanceTemp = type->CreateInstance();

        auto&& fieldMap = s_scriptEngineData->EntityScriptFieldData;

        for(auto&& field : type->GetFields())
        {
            auto&& fieldName = static_cast<std::string>(field.GetName());
            
            if (fieldName == "UUID")
            {
                continue;
            }

            auto&& fieldDataName = static_cast<std::string>(field.GetType().GetFullName());
            auto&& accessibility = field.GetAccessibility();
            
            bool hasShowField{false};
            for(auto&& attribs : field.GetAttributes()){
                if(static_cast<std::string>(attribs.GetType().GetFullName()) == "Creepy.ShowField"){
                    hasShowField = true;
                }
            }

            if (ScriptEngine::IsDataTypeExits(fieldDataName) && (accessibility == Coral::TypeAccessibility::Public || hasShowField))
            {
                auto &&fieldDataType = Utils::ConvertScriptStringToFieldType(fieldDataName);
                
                fieldMap[uuid].emplace(std::make_pair(fieldName, fieldDataType));

                entityInstanceTemp.GetFieldValueRaw(fieldName, fieldMap.at(uuid).at(fieldName).GetValueRaw());
            }
        }

        entityInstanceTemp.Destroy();
    }
}