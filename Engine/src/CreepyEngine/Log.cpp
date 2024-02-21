#include <CreepyEngine/Log.hpp>


namespace Creepy {
    std::shared_ptr<spdlog::logger> Log::s_engineLogger;

    std::shared_ptr<spdlog::logger> Log::s_clientLogger;

    void Log::Init() noexcept {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_engineLogger = spdlog::stdout_color_mt("Engine");
        s_engineLogger->set_level(spdlog::level::trace);
        s_clientLogger = spdlog::stdout_color_mt("Sandbox");
        s_clientLogger->set_level(spdlog::level::trace);
    }
}