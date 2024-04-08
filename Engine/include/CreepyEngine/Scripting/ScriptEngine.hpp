#pragma once

#include <string>
#include <string_view>
#include <filesystem>
#include <unordered_map>
#include <algorithm>
#include <cstring>
#include <cstddef>
#include <glm/glm.hpp>


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
    class ScriptField;

    enum class ScriptFieldDataType : uint32_t {
        NONE = 0, BOOL, BYTE, SBYTE, CHAR, SHORT, USHORT, INT, UINT, NINT, LONG, ULONG, FLOAT, DOUBLE, DECIMAL, STRING,
        ENTITY, VECTOR2, VECTOR3, VECTOR4
    };


    class ScriptEngine
    {
        public:
            using FieldMap = std::unordered_map<std::string, ScriptField>;

            static void Init() noexcept;

            static void ShutDown() noexcept;

            static void LoadCoreAssembly(const std::filesystem::path& filePath) noexcept;
            
            static void LoadAppAssembly(const std::filesystem::path& filePath) noexcept;

            [[nodiscard]] static Coral::ManagedAssembly& GetLoadedCoreAssembly() noexcept;

            [[nodiscard]] static Coral::ManagedAssembly& GetLoaderAppAssembly() noexcept;

            static bool IsClassExits(const std::string& fullClassName) noexcept;

            static bool IsDataTypeExits(std::string_view dataTypeName) noexcept;

            [[nodiscard]] static std::unordered_map<std::string, Coral::Type*>& GetEntityClasses() noexcept;

            [[nodiscard]] static Coral::Type* GetEntityClass(const std::string& className) noexcept;

            [[nodiscard]] static std::unordered_map<std::string_view, ScriptFieldDataType>& GetScriptFieldDataType() noexcept;

            [[nodiscard]] static std::unordered_map<UUID, FieldMap>& GetScriptFieldData() noexcept;

            static void OnRunTimeStart(Scene* scene) noexcept;

            static void OnRunTimeStop() noexcept;

            static void OnCreateEntity(Entity& entity) noexcept;

            static void OnUpdateEntity(Entity& entity, float timeStep) noexcept;

            [[nodiscard]] static Scene* GetSceneContext() noexcept;

            [[nodiscard]] static Coral::ManagedObject GetEntityInstance(UUID uuid) noexcept;

            static void CreateEntityFastInstanceToCopyData(UUID uuid, Coral::Type* type) noexcept;

        private:
            static void initCoral() noexcept;

            static void shutDownCoral() noexcept;

    };

    template<typename T, typename ... U>
    concept IsAnyOf = (std::same_as<T, U> || ...);

    template <typename T>
    concept RequireTypes = IsAnyOf<std::remove_cvref_t<T>, bool, char, uint8_t, short, uint16_t, int, 
        uint32_t, long, uint64_t, float, double, glm::vec2, glm::vec3, glm::vec4>;

    class ScriptField
    {
        public:

            constexpr ScriptField() noexcept {
                std::ranges::fill(m_buffer, 0);
            }

            constexpr ScriptField(ScriptFieldDataType dataType) noexcept : DataType{dataType} {
                std::ranges::fill(m_buffer, 0);
            }

            template <RequireTypes T>
            [[nodiscard]] constexpr T GetValue() noexcept
            {
                return *reinterpret_cast<T*>(m_buffer);
            }

            template <RequireTypes T>
            constexpr void SetValue(T value) noexcept
            {
                // std::memcpy(m_buffer, &value, sizeof(T));
                new (&m_buffer[0]) T(value);
            }

            [[nodiscard]] constexpr uint8_t* GetValueRaw() noexcept {
                return m_buffer;
            }

        public:
            ScriptFieldDataType DataType{ScriptFieldDataType::NONE};

        private:
            uint8_t m_buffer[16];
    };
    
}