#pragma once

#include <string_view>

#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Scripting/ScriptEngine.hpp>

namespace Creepy::Utils {

    static ScriptFieldDataType ConvertStringToFieldType(std::string_view typeName) noexcept {
        if(!ScriptEngine::GetScriptFieldData().contains(typeName)){
            ENGINE_LOG_ERROR("Data type not exit in map");
            return ScriptFieldDataType::NONE;
        }
        return ScriptEngine::GetScriptFieldData()[typeName];
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

        // std::unreachable();
        return "";
    }

}