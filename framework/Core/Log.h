#pragma once

#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Framework
{
    class Log
    {
    public:
        static void init();

        inline static std::shared_ptr<spdlog::logger>& getLogger()
        {
            return logger;
        }

    private:
        static std::shared_ptr<spdlog::logger> logger;
    };
}

#define TRACE(...)      Framework::Log::getLogger()->trace(__VA_ARGS__)
#define INFO(...)       Framework::Log::getLogger()->info(__VA_ARGS__)
#define DEBUG(...)      Framework::Log::getLogger()->info(__VA_ARGS__)
#define WARN(...)       Framework::Log::getLogger()->warn(__VA_ARGS__)
#define ERROR(...)      Framework::Log::getLogger()->error(__VA_ARGS__)
#define FATAL(...)      Framework::Log::getLogger()->fatal(__VA_ARGS__)
