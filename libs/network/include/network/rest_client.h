#pragma once

#include <functional>
#include <future>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace devops::network {

struct RestResponse {
    int statusCode = 0;
    nlohmann::json body;
    bool ok() const { return statusCode >= 200 && statusCode < 300; }
};

/// Client HTTP générique, volontairement sans dépendance Qt : il est
/// pensé pour être réutilisable côté backend (tests, appels vers des
/// services tiers) autant que côté client Qt/QML. `services_api.h`
/// l'enrichit avec les endpoints métier de ce projet.
class RestClient {
public:
    explicit RestClient(std::string baseUrl);

    void setBearerToken(std::string token);

    RestResponse get(const std::string& path);
    RestResponse post(const std::string& path, const nlohmann::json& body);

    /// Version asynchrone : ne bloque pas l'appelant. À utiliser côté
    /// client Qt pour ne jamais geler l'UI le temps d'un appel réseau.
    std::future<RestResponse> getAsync(const std::string& path);

private:
    std::string m_baseUrl;
    std::optional<std::string> m_bearerToken;
};

}  // namespace devops::network
