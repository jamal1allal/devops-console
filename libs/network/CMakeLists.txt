cmake_minimum_required(VERSION 3.21)
project(devops-network LANGUAGES CXX)

find_package(httplib REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(devops-core REQUIRED)
find_package(devops-protocol REQUIRED)

add_library(devops-network
    src/rest_client.cpp
    src/services_api.cpp
)
add_library(devops::network ALIAS devops-network)

target_include_directories(devops-network
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(devops-network
    PUBLIC
        devops::core
        devops::protocol
        nlohmann_json::nlohmann_json
    PRIVATE
        httplib::httplib
)

target_compile_features(devops-network PUBLIC cxx_std_20)

install(TARGETS devops-network
    EXPORT devops-network-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT devops-network-targets
    FILE devops-network-targets.cmake
    NAMESPACE devops::
    DESTINATION lib/cmake/devops-network
)
