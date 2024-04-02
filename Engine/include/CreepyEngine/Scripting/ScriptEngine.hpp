#pragma once

#include <string>
#include <string_view>
#include <filesystem>
#include <unordered_map>


// Forward declare

namespace Coral {
    class ManagedAssembly;
    class ManagedObject;
    class Type;
}

namespace Creepy{

    class Scene;
    class Entity;
    class UUID;

    enum class ScriptFieldDataType : uint32_t {
        NONE = 0, BOOL, BYTE, SBYTE, CHAR, SHORT, USHORT, INT, UINT, NINT, LONG, ULONG, FLOAT, DOUBLE, DECIMAL, STRING,
        ENTITY, VECTOR2, VECTOR3, VECTOR4
    };

    class ScriptEngine
    {
        public:
            static void Init() noexcept;

            static void ShutDown() noexcept;

            static void LoadCoreAssembly(const std::filesystem::path& filePath) noexcept;
            
            static void LoadAppAssembly(const std::filesystem::path& filePath) noexcept;

            static Coral::ManagedAssembly& GetLoadedCoreAssembly() noexcept;

            static Coral::ManagedAssembly& GetLoaderAppAssembly() noexcept;

            static bool IsClassExits(const std::string& fullClassName) noexcept;

            static std::unordered_map<std::string, Coral::Type*>& GetEntityClasses() noexcept;

            static Coral::Type* GetEntityClass(const std::string& className) noexcept;

            static std::unordered_map<std::string_view, ScriptFieldDataType>& GetScriptFieldData() noexcept;

            static void OnRunTimeStart(Scene* scene) noexcept;

            static void OnRunTimeStop() noexcept;

            static void OnCreateEntity(Entity& entity) noexcept;

            static void OnUpdateEntity(Entity& entity, float timeStep) noexcept;

            static Scene* GetSceneContext() noexcept;

            static Coral::ManagedObject* GetEntityInstance(UUID uuid) noexcept;

        private:
            static void initCoral() noexcept;

            static void shutDownCoral() noexcept;

    };

    
}