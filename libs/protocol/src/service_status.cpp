#include "protocol/service_status.h"

namespace devops::protocol {

std::string toString(HealthState state) {
    switch (state) {
        case HealthState::Healthy:  return "healthy";
        case HealthState::Degraded: return "degraded";
        case HealthState::Down:     return "down";
        case HealthState::Unknown:  return "unknown";
    }
    return "unknown";
}

HealthState healthStateFromString(const std::string& raw) {
    if (raw == "healthy") return HealthState::Healthy;
    if (raw == "degraded") return HealthState::Degraded;
    if (raw == "down") return HealthState::Down;
    return HealthState::Unknown;
}

ServiceStatus ServiceStatus::fromJson(const nlohmann::json& j) {
    ServiceStatus status;
    status.id = j.at("id").get<std::string>();
    status.name = j.at("name").get<std::string>();
    status.health = healthStateFromString(j.value("health", "unknown"));
    if (j.contains("last_deployed_at") && !j.at("last_deployed_at").is_null()) {
        status.lastDeployedAt = j.at("last_deployed_at").get<std::string>();
    }
    if (j.contains("version") && !j.at("version").is_null()) {
        status.version = j.at("version").get<std::string>();
    }
    return status;
}

nlohmann::json ServiceStatus::toJson() const {
    nlohmann::json j;
    j["id"] = id;
    j["name"] = name;
    j["health"] = toString(health);
    j["last_deployed_at"] = lastDeployedAt ? nlohmann::json(*lastDeployedAt) : nlohmann::json(nullptr);
    j["version"] = version ? nlohmann::json(*version) : nlohmann::json(nullptr);
    return j;
}

}  // namespace devops::protocol
