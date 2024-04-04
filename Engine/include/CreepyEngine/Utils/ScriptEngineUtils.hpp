#pragma once

#include <string_view>
#include <cstddef>
#include <cstdint>
#include <stdfloat>

#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Scripting/ScriptEngine.hpp>
#include <glm/glm.hpp>

namespace Creepy::Utils {

    static constexpr ScriptFieldDataType ConvertStringToFieldType(std::string_view typeName) noexcept {
        if(!ScriptEngine::GetScriptFieldDataType().contains(typeName)){
            ENGINE_LOG_ERROR("Data type not exit in map");
            return ScriptFieldDataType::NONE;
        }
        
        return ScriptEngine::GetScriptFieldDataType()[typeName];
    }

    static constexpr std::string_view ConvertFieldTypeToString(ScriptFieldDataType type) noexcept {

        switch(type){
            case ScriptFieldDataType::BOOL:
                return "bool";
            case ScriptFieldDataType::BYTE:
                return "byte";
            case ScriptFieldDataType::SBYTE:
                return "sbyte";
            case ScriptFieldDataType::CHAR:
                return "char";
            case ScriptFieldDataType::SHORT:
                return "short";
            case ScriptFieldDataType::USHORT:
                return "ushort";
            case ScriptFieldDataType::INT:
                return "int";
            case ScriptFieldDataType::UINT:
                return "uint";
            case ScriptFieldDataType::NINT:
                return "nint";
            case ScriptFieldDataType::LONG:
                return "long";
            case ScriptFieldDataType::ULONG:
                return "ulong";
            case ScriptFieldDataType::FLOAT:
                return "float";
            case ScriptFieldDataType::DOUBLE:
                return "double";
            case ScriptFieldDataType::DECIMAL:
                return "decimal";
            case ScriptFieldDataType::STRING:
                return "string";
            case ScriptFieldDataType::ENTITY:
                return "Entity";
            case ScriptFieldDataType::VECTOR2:
                return "Vector2";
            case ScriptFieldDataType::VECTOR3:
                return "Vector3";
            case ScriptFieldDataType::VECTOR4:
                return "Vector4";
        }

        return "";
    }

    static void GetScriptFieldDefaultData(ScriptField& scriptField) noexcept {

    }

    template <ScriptFieldDataType fieldType = ScriptFieldDataType::NONE>
    struct ScriptDataType{
        using type = void;
    };

    template <>
    struct ScriptDataType<ScriptFieldDataType::BOOL>{
        using type = bool;
    };

    template <>
    struct ScriptDataType<ScriptFieldDataType::BYTE>{
        using type = std::byte;
    };

    template <>
    struct ScriptDataType<ScriptFieldDataType::CHAR>{
        using type = char;
    };

    template <>
    struct ScriptDataType<ScriptFieldDataType::SHORT>{
        using type = short;
    };

    template <>
    struct ScriptDataType<ScriptFieldDataType::USHORT>{
        using type = uint16_t;
    };

    template <>
    struct ScriptDataType<ScriptFieldDataType::INT>{
        using type = int;
    };

    template <>
    struct ScriptDataType<ScriptFieldDataType::UINT>{
        using type = uint32_t;
    };

    template <>
    struct ScriptDataType<ScriptFieldDataType::LONG>{
        using type = long;
    };

    template <>
    struct ScriptDataType<ScriptFieldDataType::ULONG>{
        using type = uint64_t;
    };

    template <>
    struct ScriptDataType<ScriptFieldDataType::FLOAT>{
        using type = float;
    };

    template <>
    struct ScriptDataType<ScriptFieldDataType::DOUBLE>{
        using type = double;
    };

    // template <>
    // struct ScriptDataType<ScriptFieldDataType::DECIMAL>{
    //     using type = std::float128_t;
    // };

    template <>
    struct ScriptDataType<ScriptFieldDataType::VECTOR2>{
        using type = glm::vec2;
    };

    template <>
    struct ScriptDataType<ScriptFieldDataType::VECTOR3>{
        using type = glm::vec3;
    };

    template <>
    struct ScriptDataType<ScriptFieldDataType::VECTOR4>{
        using type = glm::vec4;
    };

}