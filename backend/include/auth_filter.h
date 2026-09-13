#pragma once

#include <drogon/HttpFilter.h>

namespace devops::backend {

/// Filtre Drogon appliqué aux routes protégées : vérifie le JWT présent
/// dans le header `Authorization: Bearer <token>`. Volontairement séparé
/// du controller pour rester réutilisable sur d'autres routes futures.
class AuthFilter : public drogon::HttpFilter<AuthFilter> {
public:
    void doFilter(const drogon::HttpRequestPtr& req,
                  drogon::FilterCallback&& failCallback,
                  drogon::FilterChainCallback&& nextCallback) override;
};

}  // namespace devops::backend
