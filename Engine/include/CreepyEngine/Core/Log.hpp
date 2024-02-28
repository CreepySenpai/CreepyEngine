#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Creepy
{
    class Log
    {

        public:

            static void Init() noexcept;

            inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() noexcept {
                return s_engineLogger;
            }

            inline static std::shared_ptr<spdlog::logger>& GetClientLogger() noexcept {
                return s_clientLogger;
            }

        private:
            static std::shared_ptr<spdlog::logger> s_engineLogger;
            static std::shared_ptr<spdlog::logger> s_clientLogger;
    };

}