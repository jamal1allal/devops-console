#include "protocol/deployment_event.h"

namespace devops::protocol {

DeploymentEvent DeploymentEvent::fromJson(const nlohmann::json& j) {
    DeploymentEvent event;
    event.serviceId = j.at("service_id").get<std::string>();
    event.version = j.at("version").get<std::string>();
    event.deployedAt = j.at("deployed_at").get<std::string>();
    event.deployedBy = j.at("deployed_by").get<std::string>();
    event.success = j.value("success", true);
    return event;
}

nlohmann::json DeploymentEvent::toJson() const {
    return nlohmann::json{
        {"service_id", serviceId},
        {"version", version},
        {"deployed_at", deployedAt},
        {"deployed_by", deployedBy},
        {"success", success},
    };
}

}  // namespace devops::protocol
