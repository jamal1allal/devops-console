cmake_minimum_required(VERSION 3.21)
project(devops-backend LANGUAGES CXX)

find_package(Drogon REQUIRED)
find_package(libpqxx REQUIRED)
find_package(jwt-cpp REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(devops-core REQUIRED)
find_package(devops-protocol REQUIRED)

add_executable(devops-backend
    src/main.cpp
    src/services_controller.cpp
    src/auth_filter.cpp
    src/repository/service_repository.cpp
)

target_include_directories(devops-backend PRIVATE include)

target_link_libraries(devops-backend
    PRIVATE
        Drogon::Drogon
        libpqxx::pqxx
        jwt-cpp::jwt-cpp
        nlohmann_json::nlohmann_json
        devops::core
        devops::protocol
)

target_compile_features(devops-backend PRIVATE cxx_std_20)
