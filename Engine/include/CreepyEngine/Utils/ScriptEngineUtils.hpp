#pragma once

#include <string_view>
#include <cstddef>
#include <cstdint>
#include <stdfloat>

#include <CreepyEngine/Core/Core.hpp>
#include <CreepyEngine/Scripting/ScriptEngine.hpp>
#include <glm/glm.hpp>

namespace Creepy::Utils {

    static constexpr ScriptFieldDataType ConvertScriptStringToFieldType(std::string_view typeName) noexcept {
        if(!ScriptEngine::GetScriptFieldDataType().contains(typeName)){
            ENGINE_LOG_ERROR("Data type not exit in map");
            return ScriptFieldDataType::NONE;
        }
        
        return ScriptEngine::GetScriptFieldDataType()[typeName];
    }

    static constexpr const char* ConvertFieldTypeToString(ScriptFieldDataType type) noexcept {

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

        return "None";
    }

    static constexpr ScriptFieldDataType ConvertStringToFieldType(std::string_view typeName) noexcept {

        if(typeName == "bool"){
            return ScriptFieldDataType::BOOL;
        }
        if(typeName == "byte"){
            return ScriptFieldDataType::BYTE;
        }
        if(typeName == "sbyte"){
            return ScriptFieldDataType::SBYTE;
        }
        if(typeName == "char"){
            return ScriptFieldDataType::CHAR;
        }
        if(typeName == "short"){
            return ScriptFieldDataType::SHORT;
        }
        if(typeName == "ushort"){
            return ScriptFieldDataType::USHORT;
        }
        if(typeName == "int"){
            return ScriptFieldDataType::INT;
        }
        if(typeName == "uint"){
            return ScriptFieldDataType::UINT;
        }
        if(typeName == "nint"){
            return ScriptFieldDataType::NINT;
        }
        if(typeName == "long"){
            return ScriptFieldDataType::LONG;
        }
        if(typeName == "ulong"){
            return ScriptFieldDataType::ULONG;
        }
        if(typeName == "float"){
            return ScriptFieldDataType::FLOAT;
        }
        if(typeName == "double"){
            return ScriptFieldDataType::DOUBLE;
        }
        if(typeName == "decimal"){
            return ScriptFieldDataType::DECIMAL;
        }
        if(typeName == "string"){
            return ScriptFieldDataType::STRING;
        }
        if(typeName == "Entity"){
            return ScriptFieldDataType::ENTITY;
        }
        if(typeName == "Vector2"){
            return ScriptFieldDataType::VECTOR2;
        }
        if(typeName == "Vector3"){
            return ScriptFieldDataType::VECTOR3;
        }
        if(typeName == "Vector4"){
            return ScriptFieldDataType::VECTOR4;
        }

        return ScriptFieldDataType::NONE;
    }
}