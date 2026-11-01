#include "auth/user_repository.h"

#include <drogon/drogon.h>

namespace devops::backend::auth {

std::optional<UserRecord> UserRepository::findByUsername(const std::string& username) {
    auto client = drogon::app().getDbClient("default");

    // Requête paramétrée ($1) : jamais de concaténation, pas d'injection SQL.
    auto res = client->execSqlSync(
        "SELECT id, username, password_hash FROM users WHERE username = $1", username);
    if (res.empty()) return std::nullopt;

    UserRecord user;
    user.id = res[0]["id"].as<int>();
    user.username = res[0]["username"].as<std::string>();
    user.passwordHash = res[0]["password_hash"].as<std::string>();
    return user;
}

}  // namespace devops::backend::auth