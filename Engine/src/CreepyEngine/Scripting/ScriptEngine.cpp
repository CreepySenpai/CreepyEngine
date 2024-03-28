#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Scripting/ScriptEngine.hpp>
#include <iostream>

#include <Coral/HostInstance.hpp>
#include <Coral/GC.hpp>
#include <Coral/Array.hpp>
#include <Coral/Attribute.hpp>

namespace Creepy {

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
        Coral::AssemblyLoadContext AssemblyContext;

    };

    static ScriptEngineData* s_scriptEngineData{nullptr};


    void ScriptEngine::Init() noexcept {

        s_scriptEngineData = new ScriptEngineData;

        initCoral();
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

        auto testDllPath = currentPath / "ScriptCore.dll";

        auto&& assembly = s_scriptEngineData->AssemblyContext.LoadAssembly(testDllPath.string());

        auto myClassType = assembly.GetType("ScriptCore.MyTest");

        auto str = Coral::String::New("OniChan");

        auto obj = myClassType.CreateInstance(str);
        
        obj.InvokeMethod("PrintName");
        
    }

    void ScriptEngine::shutDownCoral() noexcept{

        s_scriptEngineData->HostInstance.UnloadAssemblyLoadContext(s_scriptEngineData->AssemblyContext);

        Coral::GC::Collect();
    }

    

}