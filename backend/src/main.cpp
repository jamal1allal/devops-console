#include <drogon/drogon.h>

#include "core/config.h"
#include "core/logger.h"
#include "auth/jwt_service.h"

using namespace devops::core;

int main() {
    Config::instance().loadFromFile(".env");

    try {
        devops::backend::auth::JwtService::secret();  // échoue vite si absent
    } catch (const std::exception& e) {
        Logger::instance().error("main", e.what());
        return 1;
    }

    const auto dbHost = Config::instance().get("DB_HOST", "localhost");
    const auto dbPort = Config::instance().getInt("DB_PORT").value_or(5432);
    const auto dbName = Config::instance().get("DB_NAME", "devops_console");
    const auto dbUser = Config::instance().get("DB_USER", "devops");
    const auto dbPassword = Config::instance().get("DB_PASSWORD", "");
    const auto listenPort = Config::instance().getInt("BACKEND_PORT").value_or(8080);

    Logger::instance().info("main", "Starting devops-backend on port " + std::to_string(listenPort));

    drogon::app().createDbClient(
        "postgresql",
        dbHost,
        static_cast<unsigned short>(dbPort),
        dbName,
        dbUser,
        dbPassword,
        /*connectionNum=*/4,
        /*filename=*/"",
        /*name=*/"default");

    // Les controllers (services_controller.cpp) s'enregistrent eux-mêmes
    // via la macro ADD_METHOD_TO / le registre Drogon au chargement.
    drogon::app()
        .addListener("0.0.0.0", listenPort)
        .setThreadNum(4)
        .run();

    return 0;
}
