#pragma once

#include <optional>
#include <string>

namespace devops::backend::auth {

struct UserRecord {
    int id = 0;
    std::string username;
    std::string passwordHash;
};

class UserRepository {
public:
    std::optional<UserRecord> findByUsername(const std::string& username);
};

}  // namespace devops::backend::auth