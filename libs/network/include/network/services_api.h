#pragma once

#include <memory>
#include <vector>

#include "network/rest_client.h"
#include "protocol/deployment_event.h"
#include "protocol/service_status.h"

namespace devops::network {

/// Enveloppe le RestClient générique avec les endpoints métier du backend
/// DevOps Console. C'est la seule classe que `app/` devrait appeler
/// directement — elle ne connaît jamais les chemins REST bruts.
class ServicesApi {
public:
    explicit ServicesApi(std::shared_ptr<RestClient> client);

    std::vector<protocol::ServiceStatus> listServices();
    protocol::ServiceStatus getService(const std::string& id);
    std::vector<protocol::DeploymentEvent> getServiceHistory(const std::string& id);

private:
    std::shared_ptr<RestClient> m_client;
};

}  // namespace devops::network
