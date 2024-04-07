#pragma once

#include "spdlog/spdlog.h"

class Log
{
public:
    Log() = delete;

    static void init(const std::string &logFile = "");

    static void setLevel(const std::string &level);
    static void setLevel(spdlog::level::level_enum level);

    // convenient aliases
    template <typename... Args>
    static void info(const char *format, Args... args)
    {
        log(spdlog::level::level_enum::info, format, args...);
    }

    template <typename... Args>
    static void warn(const char *format, Args... args)
    {
        log(spdlog::level::level_enum::warn, format, args...);
    }

    template <typename... Args>
    static void error(const char *format, Args... args)
    {
        log(spdlog::level::level_enum::err, format, args...);
    }

private:
    template <typename... Args>
    static void log(spdlog::level::level_enum level, const char *format, Args... args)
    {
        if (logger_ != nullptr)
            logger_->log(level, format, args...);
    }

    static std::shared_ptr<spdlog::logger> logger_;
};
