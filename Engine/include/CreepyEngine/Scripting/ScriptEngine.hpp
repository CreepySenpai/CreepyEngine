#pragma once

#include <string>
#include <string_view>
#include <filesystem>

#include <Coral/Type.hpp>

// Forward declare
namespace Coral {
    class ManagedAssembly;
    class ManagedObject;
}

namespace Creepy{

    class ScriptEngine
    {
        public:
            static void Init() noexcept;

            static void ShutDown() noexcept;

            static void LoadAssembly(const std::filesystem::path& filePath) noexcept;

            static Coral::ManagedAssembly& GetLoadedAssembly() noexcept;

        private:
            static void initCoral() noexcept;

            static void shutDownCoral() noexcept;

    };

    class ScriptClass {
        
        public:
            ScriptClass(const std::string& classNameSpace,const std::string& className) noexcept;

            ScriptClass() noexcept = default;

            template <typename... Args>
            Coral::ManagedObject Instantiate(Args&&... args) noexcept {
                return m_type.CreateInstance(std::forward<Args>(args)...);
            }

        private:
            std::string m_classNameSpace;
            std::string m_className;
            Coral::Type m_type;
    };
}