#include "Log.hpp"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

// static members
std::shared_ptr<spdlog::logger> Log::logger_ = nullptr;

void Log::init(const std::string &logFile)
{
    std::vector<spdlog::sink_ptr> sinks;

    auto consoleLogger = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
    // timestamp with ms, colored log level, message
    consoleLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    sinks.push_back(consoleLogger);

    if (!logFile.empty())
    {
        constexpr bool TRUNCATE = true;
        try
        {
            auto fileLogger = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile, TRUNCATE);
            // timestamp with ms, log level, message
            fileLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
            sinks.push_back(fileLogger);
        }
        catch (const spdlog::spdlog_ex &ex)
        {
            spdlog::error("Failed to create file logger: {}", ex.what());
        }
    }

    logger_ = std::make_shared<spdlog::logger>("logger", sinks.begin(), sinks.end());
    spdlog::register_logger(logger_);
}

void Log::setLevel(const std::string &level)
{
    constexpr auto DEFAULT_LEVEL = spdlog::level::info;
    spdlog::level::level_enum finalLevel = DEFAULT_LEVEL;

    if (level == "off")
        finalLevel = spdlog::level::off;
    else if (level == "error")
        finalLevel = spdlog::level::err;
    else if (level == "warning")
        finalLevel = spdlog::level::warn;
    else if (level == "info")
        finalLevel = spdlog::level::info;

    setLevel(finalLevel);
}

void Log::setLevel(spdlog::level::level_enum level)
{
    if (logger_ != nullptr)
        logger_->set_level(level);
}
