#include "core/logger.h"

#include <chrono>
#include <ctime>
#include <iostream>

namespace devops::core {

namespace {
std::string_view levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:   return "DEBUG";
        case LogLevel::Info:    return "INFO";
        case LogLevel::Warning: return "WARN";
        case LogLevel::Error:   return "ERROR";
    }
    return "?";
}

std::string timestampNow() {
    const auto now = std::chrono::system_clock::now();
    const auto t = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    return buf;
}
}  // namespace

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

void Logger::setMinLevel(LogLevel level) {
    std::lock_guard lock(m_mutex);
    m_minLevel = level;
}

void Logger::log(LogLevel level, std::string_view component, std::string_view message) {
    std::lock_guard lock(m_mutex);
    if (level < m_minLevel) return;

    auto& stream = (level == LogLevel::Error) ? std::cerr : std::cout;
    stream << "[" << timestampNow() << "] [" << levelToString(level) << "] "
           << "[" << component << "] " << message << '\n';
}

void Logger::debug(std::string_view component, std::string_view message) {
    log(LogLevel::Debug, component, message);
}
void Logger::info(std::string_view component, std::string_view message) {
    log(LogLevel::Info, component, message);
}
void Logger::warning(std::string_view component, std::string_view message) {
    log(LogLevel::Warning, component, message);
}
void Logger::error(std::string_view component, std::string_view message) {
    log(LogLevel::Error, component, message);
}

}  // namespace devops::core
