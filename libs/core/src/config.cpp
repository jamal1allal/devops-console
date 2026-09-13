#include "core/config.h"

#include <cstdlib>
#include <fstream>
#include <sstream>

namespace devops::core {

Config& Config::instance() {
    static Config config;
    return config;
}

bool Config::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        const auto pos = line.find('=');
        if (pos == std::string::npos) continue;
        const auto key = line.substr(0, pos);
        const auto value = line.substr(pos + 1);
        m_values[key] = value;
    }
    return true;
}

std::string Config::get(const std::string& key, const std::string& defaultValue) const {
    if (auto it = m_values.find(key); it != m_values.end()) {
        return it->second;
    }
    if (const char* envValue = std::getenv(key.c_str())) {
        return envValue;
    }
    return defaultValue;
}

std::optional<int> Config::getInt(const std::string& key) const {
    const auto raw = get(key);
    if (raw.empty()) return std::nullopt;
    try {
        return std::stoi(raw);
    } catch (...) {
        return std::nullopt;
    }
}

void Config::set(const std::string& key, const std::string& value) {
    m_values[key] = value;
}

}  // namespace devops::core
