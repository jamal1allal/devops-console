#pragma once

#include <drogon/HttpController.h>

#include "auth/user_repository.h"

namespace devops::backend::auth {

///   POST /api/v1/auth/login   {username, password}  ->  {token, ...}
class AuthController : public drogon::HttpController<AuthController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(AuthController::login, "/api/v1/auth/login", drogon::Post);
    METHOD_LIST_END

    void login(const drogon::HttpRequestPtr& req,
               std::function<void(const drogon::HttpResponsePtr&)>&& callback);

private:
    UserRepository m_users;
};

}  // namespace devops::backend::auth