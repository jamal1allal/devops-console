#include "auth/auth_controller.h"

#include <nlohmann/json.hpp>

#include "auth/jwt_service.h"
#include "auth/password_hasher.h"
#include "core/logger.h"

namespace devops::backend::auth {

using devops::core::Logger;

namespace {
drogon::HttpResponsePtr jsonResponse(const nlohmann::json& json,
                                     drogon::HttpStatusCode status = drogon::k200OK) {
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setStatusCode(status);
    resp->setContentTypeCode(drogon::CT_APPLICATION_JSON);
    resp->setBody(json.dump());
    return resp;
}

drogon::HttpResponsePtr errorResponse(drogon::HttpStatusCode status,
                                      const std::string& code, const std::string& message) {
    return jsonResponse({{"error", {{"code", code}, {"message", message}}}}, status);
}
}  // namespace

void AuthController::login(const drogon::HttpRequestPtr& req,
                           std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
    std::string username, password;
    try {
        const auto body = nlohmann::json::parse(req->getBody());
        username = body.at("username").get<std::string>();
        password = body.at("password").get<std::string>();
    } catch (const std::exception&) {
        callback(errorResponse(drogon::k400BadRequest, "invalid_request",
                               "Body must be JSON with 'username' and 'password'"));
        return;
    }

    try {
        const auto user = m_users.findByUsername(username);

        // Même réponse que l'utilisateur n'existe pas ou que le mot de passe
        // soit faux : on ne révèle pas quels comptes existent.
        if (!user || !PasswordHasher::verify(user->passwordHash, password)) {
            Logger::instance().warning("AuthController", "failed login for user '" + username + "'");
            callback(errorResponse(drogon::k401Unauthorized, "invalid_credentials",
                                   "Invalid username or password"));
            return;
        }

        callback(jsonResponse({{"token", JwtService::createToken(user->id)},
                               {"token_type", "Bearer"},
                               {"expires_in", JwtService::kTokenLifetimeSeconds}}));
    } catch (const std::exception& e) {
        Logger::instance().error("AuthController", std::string("login failed: ") + e.what());
        callback(errorResponse(drogon::k500InternalServerError, "internal_error", "Login failed"));
    }
}

}  // namespace devops::backend::auth