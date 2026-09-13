#include "auth_filter.h"

#include <jwt-cpp/jwt.h>

#include "core/config.h"
#include "core/logger.h"

namespace devops::backend {

using devops::core::Config;
using devops::core::Logger;

void AuthFilter::doFilter(const drogon::HttpRequestPtr& req,
                           drogon::FilterCallback&& failCallback,
                           drogon::FilterChainCallback&& nextCallback) {
    const auto authHeader = req->getHeader("Authorization");
    static constexpr std::string_view prefix = "Bearer ";

    if (authHeader.size() <= prefix.size() || authHeader.compare(0, prefix.size(), prefix) != 0) {
        Logger::instance().warning("AuthFilter", "missing or malformed Authorization header");
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k401Unauthorized);
        failCallback(resp);
        return;
    }

    const auto token = authHeader.substr(prefix.size());
    const auto secret = Config::instance().get("JWT_SECRET", "dev-secret-change-me");

    try {
        const auto decoded = jwt::decode(token);
        const auto verifier = jwt::verify()
                                   .allow_algorithm(jwt::algorithm::hs256{secret})
                                   .with_issuer("devops-console-backend");
        verifier.verify(decoded);
        nextCallback();
    } catch (const std::exception& e) {
        Logger::instance().warning("AuthFilter", std::string("invalid token: ") + e.what());
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k401Unauthorized);
        failCallback(resp);
    }
}

}  // namespace devops::backend
