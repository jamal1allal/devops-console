#pragma once

#include <string>

namespace devops::backend::auth {

class JwtService {
public:
    static constexpr int kTokenLifetimeSeconds = 24 * 3600;

    /// Lit JWT_SECRET. Lève une exception si absent ou trop court :
    /// on préfère refuser de démarrer plutôt que signer avec un secret faible.
    static std::string secret();

    static std::string createToken(int userId);
};

}  // namespace devops::backend::auth