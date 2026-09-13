cmake_minimum_required(VERSION 3.21)
project(devops-protocol LANGUAGES CXX)

find_package(nlohmann_json REQUIRED)
find_package(devops-core REQUIRED)

add_library(devops-protocol
    src/service_status.cpp
    src/deployment_event.cpp
)
add_library(devops::protocol ALIAS devops-protocol)

target_include_directories(devops-protocol
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(devops-protocol
    PUBLIC
        nlohmann_json::nlohmann_json
        devops::core
)

target_compile_features(devops-protocol PUBLIC cxx_std_20)

install(TARGETS devops-protocol
    EXPORT devops-protocol-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT devops-protocol-targets
    FILE devops-protocol-targets.cmake
    NAMESPACE devops::
    DESTINATION lib/cmake/devops-protocol
)
