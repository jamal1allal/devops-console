#pragma once

#include <mutex>
#include <string>
#include <string_view>

namespace devops::core {

enum class LogLevel { Debug, Info, Warning, Error };

/// Logger process-wide, thread-safe, minimal mais suffisant pour tracer
/// les appels réseau et le cycle de vie des services. Volontairement
/// simple : dans une vraie équipe, on le remplacerait par spdlog, mais
/// l'objectif ici est de garder `core` sans dépendance externe.
class Logger {
public:
    static Logger& instance();

    void setMinLevel(LogLevel level);
    void log(LogLevel level, std::string_view component, std::string_view message);

    void debug(std::string_view component, std::string_view message);
    void info(std::string_view component, std::string_view message);
    void warning(std::string_view component, std::string_view message);
    void error(std::string_view component, std::string_view message);

private:
    Logger() = default;

    std::mutex m_mutex;
    LogLevel m_minLevel = LogLevel::Info;
};

}  // namespace devops::core
