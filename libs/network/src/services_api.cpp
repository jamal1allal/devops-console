#include "network/services_api.h"

#include <stdexcept>

namespace devops::network {

ServicesApi::ServicesApi(std::shared_ptr<RestClient> client) : m_client(std::move(client)) {}

std::vector<protocol::ServiceStatus> ServicesApi::listServices() {
    const auto response = m_client->get("/api/v1/services");
    if (!response.ok()) {
        throw std::runtime_error("listServices failed with status " + std::to_string(response.statusCode));
    }

    std::vector<protocol::ServiceStatus> services;
    for (const auto& item : response.body.at("services")) {
        services.push_back(protocol::ServiceStatus::fromJson(item));
    }
    return services;
}

protocol::ServiceStatus ServicesApi::getService(const std::string& id) {
    const auto response = m_client->get("/api/v1/services/" + id);
    if (!response.ok()) {
        throw std::runtime_error("getService(" + id + ") failed with status " + std::to_string(response.statusCode));
    }
    return protocol::ServiceStatus::fromJson(response.body);
}

std::vector<protocol::DeploymentEvent> ServicesApi::getServiceHistory(const std::string& id) {
    const auto response = m_client->get("/api/v1/services/" + id + "/events");
    if (!response.ok()) {
        throw std::runtime_error("getServiceHistory(" + id + ") failed with status " + std::to_string(response.statusCode));
    }

    std::vector<protocol::DeploymentEvent> events;
    for (const auto& item : response.body.at("events")) {
        events.push_back(protocol::DeploymentEvent::fromJson(item));
    }
    return events;
}

}  // namespace devops::network
