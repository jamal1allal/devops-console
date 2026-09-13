#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace devops::protocol {

/// Un événement de déploiement, tel que stocké en historique et exposé
/// via `GET /api/v1/services/{id}/events`.
struct DeploymentEvent {
    std::string serviceId;
    std::string version;
    std::string deployedAt;   // ISO 8601
    std::string deployedBy;
    bool success = true;

    static DeploymentEvent fromJson(const nlohmann::json& j);
    nlohmann::json toJson() const;
};

}  // namespace devops::protocol
