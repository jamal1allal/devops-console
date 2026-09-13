#include "network/rest_client.h"

#include <httplib.h>

#include "core/logger.h"

namespace devops::network {

namespace {
// Sépare "https://host:port" en (host, port, is_ssl) pour httplib::Client.
struct ParsedUrl {
    std::string host;
    int port;
    bool ssl;
};

ParsedUrl parseBaseUrl(const std::string& url) {
    ParsedUrl result{};
    result.ssl = url.starts_with("https://");
    auto withoutScheme = url.substr(url.find("://") + 3);
    const auto colonPos = withoutScheme.find(':');
    if (colonPos != std::string::npos) {
        result.host = withoutScheme.substr(0, colonPos);
        result.port = std::stoi(withoutScheme.substr(colonPos + 1));
    } else {
        result.host = withoutScheme;
        result.port = result.ssl ? 443 : 80;
    }
    return result;
}
}  // namespace

RestClient::RestClient(std::string baseUrl) : m_baseUrl(std::move(baseUrl)) {}

void RestClient::setBearerToken(std::string token) {
    m_bearerToken = std::move(token);
}

RestResponse RestClient::get(const std::string& path) {
    const auto parsed = parseBaseUrl(m_baseUrl);
    httplib::Client client(parsed.host, parsed.port);
    client.set_connection_timeout(5);

    httplib::Headers headers;
    if (m_bearerToken) {
        headers.emplace("Authorization", "Bearer " + *m_bearerToken);
    }

    auto res = client.Get(path, headers);
    RestResponse response;
    if (!res) {
        devops::core::Logger::instance().error("RestClient", "GET " + path + " failed: no response");
        response.statusCode = 0;
        return response;
    }

    response.statusCode = res->status;
    if (!res->body.empty()) {
        try {
            response.body = nlohmann::json::parse(res->body);
        } catch (const std::exception& e) {
            devops::core::Logger::instance().warning("RestClient", std::string("failed to parse JSON body: ") + e.what());
        }
    }
    return response;
}

RestResponse RestClient::post(const std::string& path, const nlohmann::json& body) {
    const auto parsed = parseBaseUrl(m_baseUrl);
    httplib::Client client(parsed.host, parsed.port);
    client.set_connection_timeout(5);

    httplib::Headers headers;
    if (m_bearerToken) {
        headers.emplace("Authorization", "Bearer " + *m_bearerToken);
    }

    auto res = client.Post(path, headers, body.dump(), "application/json");
    RestResponse response;
    if (!res) {
        devops::core::Logger::instance().error("RestClient", "POST " + path + " failed: no response");
        return response;
    }

    response.statusCode = res->status;
    if (!res->body.empty()) {
        try {
            response.body = nlohmann::json::parse(res->body);
        } catch (const std::exception& e) {
            devops::core::Logger::instance().warning("RestClient", std::string("failed to parse JSON body: ") + e.what());
        }
    }
    return response;
}

std::future<RestResponse> RestClient::getAsync(const std::string& path) {
    return std::async(std::launch::async, [this, path]() { return get(path); });
}

}  // namespace devops::network
