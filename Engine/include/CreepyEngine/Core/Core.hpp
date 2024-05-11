#pragma once

#include <memory>
#include "Log.hpp"
#include <CreepyEngine/Debug/Instrumentor.hpp>

// Some Macro Here


// Engine Log
#define ENGINE_LOG_ERROR(...) Creepy::Log::GetEngineLogger()->error(__VA_ARGS__)

#define ENGINE_LOG_WARNING(...) Creepy::Log::GetEngineLogger()->warn(__VA_ARGS__)

#define ENGINE_LOG_INFO(...) Creepy::Log::GetEngineLogger()->info(__VA_ARGS__)

#define ENGINE_LOG_TRACE(...) Creepy::Log::GetEngineLogger()->trace(__VA_ARGS__)


// App Log
#define APP_LOG_ERROR(...) Creepy::Log::GetClientLogger()->error(__VA_ARGS__)

#define APP_LOG_WARNING(...) Creepy::Log::GetClientLogger()->warn(__VA_ARGS__)

#define APP_LOG_INFO(...) Creepy::Log::GetClientLogger()->info(__VA_ARGS__)

#define APP_LOG_TRACE(...) Creepy::Log::GetClientLogger()->trace(__VA_ARGS__)

#define ENGINE_ASSERT_C(cond)    \
if(!cond){                              \
    ENGINE_LOG_WARNING("Failed in file {}, at line {}", __FILE__, __LINE__);               \
    __debugbreak();                     \
}  

#define ENGINE_ASSERT(cond, message)    \
if(!cond){                              \
    ENGINE_LOG_WARNING("Failed to {}, in file {}, at line {}", message, __FILE__, __LINE__);               \
    __debugbreak();                     \
}                                       \


namespace Creepy {
    
    template <typename T, size_t Size>
    static consteval inline size_t GetArraySize(T (&array)[Size]) {
        return Size;
    }

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T>
    using Scope = std::unique_ptr<T>;



    inline constexpr bool UseOpenGLAPI = false;

    inline constexpr bool UseVulkanAPI = true;
}


