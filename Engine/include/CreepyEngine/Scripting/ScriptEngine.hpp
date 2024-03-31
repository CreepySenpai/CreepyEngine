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

    class ScriptEngine
    {
        public:
            static void Init() noexcept;

            static void ShutDown() noexcept;

            static void LoadAssembly(const std::filesystem::path& filePath) noexcept;

            static Coral::ManagedAssembly& GetLoadedAssembly() noexcept;

            static bool IsClassExits(const std::string& fullClassName) noexcept;

            static std::unordered_map<std::string, Coral::Type*>& GetEntityClasses() noexcept;

            static void OnRunTimeStart(Scene* scene) noexcept;
            

            static void OnRunTimeStop() noexcept;

            static void OnCreateEntity(Entity& entity) noexcept;

            static void OnUpdateEntity(Entity& entity, float timeStep) noexcept;

            static Scene* GetSceneContext() noexcept;

        private:
            static void initCoral() noexcept;

            static void shutDownCoral() noexcept;

    };
}