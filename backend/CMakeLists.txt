cmake_minimum_required(VERSION 3.21)
project(devops-backend LANGUAGES CXX)

find_package(Drogon REQUIRED)
find_package(libpqxx REQUIRED)
find_package(jwt-cpp REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(devops-core REQUIRED)
find_package(devops-protocol REQUIRED)
find_package(libsodium REQUIRED)

# Module d'authentification : lib partagée entre l'exécutable et les tests
add_library(devops-backend-auth STATIC src/auth/password_hasher.cpp)
target_include_directories(devops-backend-auth PUBLIC include)
target_link_libraries(devops-backend-auth PUBLIC libsodium::libsodium)
target_compile_features(devops-backend-auth PUBLIC cxx_std_20)

add_executable(devops-hash-password tools/hash_password.cpp)
target_link_libraries(devops-hash-password PRIVATE devops-backend-auth)

add_executable(devops-backend
    src/main.cpp
    src/services_controller.cpp
    src/auth_filter.cpp
    src/repository/service_repository.cpp
    src/auth/user_repository.cpp
    src/auth/jwt_service.cpp
    src/auth/auth_controller.cpp
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
        devops-backend-auth
)

target_compile_features(devops-backend PRIVATE cxx_std_20)

# Tests
find_package(GTest REQUIRED)
enable_testing()

add_executable(devops-backend-tests tests/test_password_hasher.cpp)
target_link_libraries(devops-backend-tests
    PRIVATE
        devops-backend-auth
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(devops-backend-tests)