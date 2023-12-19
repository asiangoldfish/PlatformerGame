#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Framework {
    std::shared_ptr<spdlog::logger> Log::logger;

    void Log::init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        logger = spdlog::stdout_color_mt("console");
        logger->set_level(spdlog::level::info);
    }
}
