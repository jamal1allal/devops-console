cmake_minimum_required(VERSION 3.21)
project(devops-core LANGUAGES CXX)

add_library(devops-core
    src/logger.cpp
    src/event_bus.cpp
    src/config.cpp
)
add_library(devops::core ALIAS devops-core)

target_include_directories(devops-core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(devops-core PUBLIC cxx_std_20)

set_target_properties(devops-core PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

install(TARGETS devops-core
    EXPORT devops-core-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT devops-core-targets
    FILE devops-core-targets.cmake
    NAMESPACE devops::
    DESTINATION lib/cmake/devops-core
)

if(DEVOPS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
