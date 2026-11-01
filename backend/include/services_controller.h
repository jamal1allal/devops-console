#pragma once

#include <drogon/HttpController.h>

#include "repository/service_repository.h"

namespace devops::backend {

/// Expose le contrat REST consommé par `libs/network` côté client :
///   GET  /api/v1/services            -> liste des services + statut
///   GET  /api/v1/services/{id}       -> détail d'un service
///   GET  /api/v1/services/{id}/events -> historique de déploiement
///   POST /api/v1/services/{id}/deploy -> enregistre un nouveau déploiement
class ServicesController : public drogon::HttpController<ServicesController> {
public:
    
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ServicesController::listServices, "/api/v1/services", drogon::Get, "devops::backend::AuthFilter");
    ADD_METHOD_TO(ServicesController::getService, "/api/v1/services/{id}", drogon::Get, "devops::backend::AuthFilter");
    ADD_METHOD_TO(ServicesController::getHistory, "/api/v1/services/{id}/events", drogon::Get, "devops::backend::AuthFilter");
    ADD_METHOD_TO(ServicesController::recordDeploy, "/api/v1/services/{id}/deploy", drogon::Post, "devops::backend::AuthFilter");
    METHOD_LIST_END

    void listServices(const drogon::HttpRequestPtr& req,
                       std::function<void(const drogon::HttpResponsePtr&)>&& callback);

    void getService(const drogon::HttpRequestPtr& req,
                     std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                     std::string id);

    void getHistory(const drogon::HttpRequestPtr& req,
                     std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                     std::string id);

    void recordDeploy(const drogon::HttpRequestPtr& req,
                       std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                       std::string id);

private:
    ServiceRepository m_repository;
};

}  // namespace devops::backend
