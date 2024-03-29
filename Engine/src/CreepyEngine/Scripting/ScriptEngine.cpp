#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Scripting/ScriptEngine.hpp>
#include <CreepyEngine/Scripting/ScriptGlue.hpp>

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


    // Forward Declare

    struct ScriptEngineData {

        Coral::HostSettings HostSetting;
        Coral::HostInstance HostInstance;
        Coral::AssemblyLoadContext AssemblyContext;
        Coral::ManagedAssembly ManagedAssembly;

    };

    static ScriptEngineData* s_scriptEngineData{nullptr};


    void ScriptEngine::Init() noexcept {

        s_scriptEngineData = new ScriptEngineData;

        initCoral();

        LoadAssembly(std::filesystem::current_path() / "ScriptCore.dll");

        ScriptGlue::RegisterFunctions();

        ScriptClass mayMain{"ScriptCore", "Main"};
        auto str = Coral::String::New("OniChan");

        auto obj = mayMain.Instantiate(str);

        obj.InvokeMethod("PrintInternal", 1);
        obj.InvokeMethod("PrintInternal", 2);
        obj.InvokeMethod("PrintInternal", 3);
        obj.InvokeMethod("PrintInternal", 4);

        obj.InvokeMethod("CallPrintConstVec3");

        Coral::String::Free(str);
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
        s_scriptEngineData->ManagedAssembly = s_scriptEngineData->AssemblyContext.LoadAssembly(filePath.string());
    }

    Coral::ManagedAssembly& ScriptEngine::GetLoadedAssembly() noexcept {
        return s_scriptEngineData->ManagedAssembly;
    }
    
    // Script Class

    ScriptClass::ScriptClass(const std::string& classNameSpace, const std::string& className) noexcept : m_classNameSpace{classNameSpace}, m_className{className}
    {
        m_type = s_scriptEngineData->ManagedAssembly.GetType(m_classNameSpace + "." + m_className);
    }

}