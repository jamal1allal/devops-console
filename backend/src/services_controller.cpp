#include "services_controller.h"

#include "core/logger.h"
#include "protocol/deployment_event.h"

namespace devops::backend {

using devops::core::Logger;
using devops::protocol::DeploymentEvent;

void ServicesController::listServices(const drogon::HttpRequestPtr& req,
                                       std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
    try {
        const auto services = m_repository.findAll();
        nlohmann::json body;
        body["services"] = nlohmann::json::array();
        for (const auto& service : services) {
            body["services"].push_back(service.toJson());
        }
        auto resp = drogon::HttpResponse::newHttpJsonResponse(body);
        callback(resp);
    } catch (const std::exception& e) {
        Logger::instance().error("ServicesController", std::string("listServices failed: ") + e.what());
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k500InternalServerError);
        callback(resp);
    }
}

void ServicesController::getService(const drogon::HttpRequestPtr& req,
                                     std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                                     std::string id) {
    try {
        const auto service = m_repository.findById(id);
        auto resp = drogon::HttpResponse::newHttpJsonResponse(service.toJson());
        callback(resp);
    } catch (const std::exception& e) {
        Logger::instance().warning("ServicesController", "service not found: " + id);
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k404NotFound);
        callback(resp);
    }
}

void ServicesController::getHistory(const drogon::HttpRequestPtr& req,
                                     std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                                     std::string id) {
    const auto events = m_repository.findEventsForService(id);
    nlohmann::json body;
    body["events"] = nlohmann::json::array();
    for (const auto& event : events) {
        body["events"].push_back(event.toJson());
    }
    auto resp = drogon::HttpResponse::newHttpJsonResponse(body);
    callback(resp);
}

void ServicesController::recordDeploy(const drogon::HttpRequestPtr& req,
                                       std::function<void(const drogon::HttpResponsePtr&)>&& callback,
                                       std::string id) {
    try {
        auto json = nlohmann::json::parse(req->getBody());
        json["service_id"] = id;
        const auto event = DeploymentEvent::fromJson(json);
        m_repository.recordDeployment(event);

        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k201Created);
        callback(resp);
    } catch (const std::exception& e) {
        Logger::instance().error("ServicesController", std::string("recordDeploy failed: ") + e.what());
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
    }
}

}  // namespace devops::backend
