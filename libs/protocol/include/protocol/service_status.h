#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace devops::protocol {

enum class HealthState { Healthy, Degraded, Down, Unknown };

/// Contrat d'API : représente l'état d'un service tel qu'exposé par
/// `GET /api/v1/services` et `GET /api/v1/services/{id}`.
///
/// Ce type vit dans `protocol` (et non dans `network` ou `app`) car
/// backend ET client en ont besoin de façon identique : c'est le contrat
/// d'interface entre les deux, versionné indépendamment du code métier
/// de chaque côté.
struct ServiceStatus {
    std::string id;
    std::string name;
    HealthState health = HealthState::Unknown;
    std::optional<std::string> lastDeployedAt;  // ISO 8601, absent si jamais déployé
    std::optional<std::string> version;

    static ServiceStatus fromJson(const nlohmann::json& j);
    nlohmann::json toJson() const;
};

std::string toString(HealthState state);
HealthState healthStateFromString(const std::string& raw);

}  // namespace devops::protocol
