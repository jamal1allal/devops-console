#pragma once

#include <vector>

#include "protocol/deployment_event.h"
#include "protocol/service_status.h"

namespace devops::backend {

/// Isole tout le SQL dans une seule classe : les controllers ne touchent
/// jamais à pqxx directement. Ça garde `services_controller.cpp` lisible
/// et rend le repository facilement testable/mockable.
class ServiceRepository {
public:
    std::vector<protocol::ServiceStatus> findAll();
    protocol::ServiceStatus findById(const std::string& id);
    std::vector<protocol::DeploymentEvent> findEventsForService(const std::string& serviceId);
    void recordDeployment(const protocol::DeploymentEvent& event);
};

}  // namespace devops::backend
