#include "auth_filter.h"

#include <jwt-cpp/traits/nlohmann-json/traits.h>

#include <string_view>

#include "auth/jwt_service.h"
#include "core/logger.h"

namespace devops::backend {

using devops::core::Logger;

namespace {
void reject(drogon::FilterCallback& failCallback) {
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k401Unauthorized);
    failCallback(resp);
}
}  // namespace

void AuthFilter::doFilter(const drogon::HttpRequestPtr& req,
                          drogon::FilterCallback&& failCallback,
                          drogon::FilterChainCallback&& nextCallback) {
    const auto authHeader = req->getHeader("Authorization");
    static constexpr std::string_view prefix = "Bearer ";

    if (authHeader.size() <= prefix.size() || authHeader.compare(0, prefix.size(), prefix) != 0) {
        Logger::instance().warning("AuthFilter", "missing or malformed Authorization header");
        reject(failCallback);
        return;
    }

    const auto token = authHeader.substr(prefix.size());

    bool valid = false;
    try {
        using traits = jwt::traits::nlohmann_json;

        const auto decoded = jwt::decode<traits>(token);
        const auto verifier = jwt::verify<traits>()
                                  .allow_algorithm(jwt::algorithm::hs256{auth::JwtService::secret()})
                                  .with_issuer("devops-console-backend");
        verifier.verify(decoded);  // vérifie signature, issuer et expiration (exp)
        valid = true;
    } catch (const std::exception& e) {
        Logger::instance().warning("AuthFilter", std::string("invalid token: ") + e.what());
    }

    if (valid) {
        nextCallback();
    } else {
        reject(failCallback);
    }
}

}  // namespace devops::backend