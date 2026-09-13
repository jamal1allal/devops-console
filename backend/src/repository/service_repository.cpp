#include "repository/service_repository.h"

#include <drogon/drogon.h>

#include "core/logger.h"

namespace devops::backend {

using devops::core::Logger;
using devops::protocol::DeploymentEvent;
using devops::protocol::HealthState;
using devops::protocol::ServiceStatus;

namespace {
ServiceStatus rowToStatus(const drogon::orm::Row& row) {
    ServiceStatus status;
    status.id = row["id"].as<std::string>();
    status.name = row["name"].as<std::string>();
    status.health = devops::protocol::healthStateFromString(row["health"].as<std::string>());
    if (!row["last_deployed_at"].isNull()) {
        status.lastDeployedAt = row["last_deployed_at"].as<std::string>();
    }
    if (!row["version"].isNull()) {
        status.version = row["version"].as<std::string>();
    }
    return status;
}
}  // namespace

std::vector<ServiceStatus> ServiceRepository::findAll() {
    auto client = drogon::app().getDbClient("default");
    std::vector<ServiceStatus> result;

    try {
        auto res = client->execSqlSync(
            "SELECT id, name, health, last_deployed_at, version FROM services ORDER BY name");
        result.reserve(res.size());
        for (const auto& row : res) {
            result.push_back(rowToStatus(row));
        }
    } catch (const std::exception& e) {
        Logger::instance().error("ServiceRepository", std::string("findAll failed: ") + e.what());
        throw;
    }
    return result;
}

ServiceStatus ServiceRepository::findById(const std::string& id) {
    auto client = drogon::app().getDbClient("default");

    auto res = client->execSqlSync(
        "SELECT id, name, health, last_deployed_at, version FROM services WHERE id = $1", id);
    if (res.empty()) {
        throw std::runtime_error("service not found: " + id);
    }
    return rowToStatus(res[0]);
}

std::vector<DeploymentEvent> ServiceRepository::findEventsForService(const std::string& serviceId) {
    auto client = drogon::app().getDbClient("default");
    std::vector<DeploymentEvent> events;

    auto res = client->execSqlSync(
        "SELECT service_id, version, deployed_at, deployed_by, success "
        "FROM deployment_events WHERE service_id = $1 ORDER BY deployed_at DESC",
        serviceId);

    for (const auto& row : res) {
        DeploymentEvent event;
        event.serviceId = row["service_id"].as<std::string>();
        event.version = row["version"].as<std::string>();
        event.deployedAt = row["deployed_at"].as<std::string>();
        event.deployedBy = row["deployed_by"].as<std::string>();
        event.success = row["success"].as<bool>();
        events.push_back(std::move(event));
    }
    return events;
}

void ServiceRepository::recordDeployment(const DeploymentEvent& event) {
    auto client = drogon::app().getDbClient("default");

    client->execSqlSync(
        "INSERT INTO deployment_events (service_id, version, deployed_at, deployed_by, success) "
        "VALUES ($1, $2, $3, $4, $5)",
        event.serviceId, event.version, event.deployedAt, event.deployedBy, event.success);

    client->execSqlSync(
        "UPDATE services SET version = $1, last_deployed_at = $2 WHERE id = $3",
        event.version, event.deployedAt, event.serviceId);
}

}  // namespace devops::backend
