#include "auth/jwt_service.h"

#include <jwt-cpp/traits/nlohmann-json/traits.h>

#include <chrono>
#include <stdexcept>

#include "core/config.h"

namespace devops::backend::auth {

std::string JwtService::secret() {
    const auto value = devops::core::Config::instance().get("JWT_SECRET");
    if (value.size() < 32) {
        throw std::runtime_error("JWT_SECRET must be set (at least 32 characters)");
    }
    return value;
}

std::string JwtService::createToken(int userId) {
    using traits = jwt::traits::nlohmann_json;
    const auto now = std::chrono::system_clock::now();

    return jwt::create<traits>()
        .set_type("JWT")
        .set_issuer("devops-console-backend")  // doit correspondre à AuthFilter
        .set_subject(std::to_string(userId))
        .set_issued_at(now)
        .set_expires_at(now + std::chrono::seconds(kTokenLifetimeSeconds))
        .sign(jwt::algorithm::hs256{secret()});
}

}  // namespace devops::backend::auth