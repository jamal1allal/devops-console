cmake_minimum_required(VERSION 3.21)
project(devops-console LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(DEVOPS_BUILD_LIBS "Build the internal libs from source (dev mode)" OFF)
option(DEVOPS_BUILD_BACKEND "Build the REST backend" ON)
option(DEVOPS_BUILD_APP "Build the Qt/QML client" ON)
option(DEVOPS_BUILD_TESTS "Build unit tests" ON)

# En mode "équipe", libs/ n'est PAS buildé ici : backend/ et app/ consomment
# les libs comme des packages Conan publiés sur le dépôt privé.
# DEVOPS_BUILD_LIBS=ON permet de tout builder en local pendant le dev initial,
# avant la première publication Conan.
if(DEVOPS_BUILD_LIBS)
    add_subdirectory(libs/core)
    add_subdirectory(libs/protocol)
    add_subdirectory(libs/network)
    add_subdirectory(libs/ui-components)
endif()

if(DEVOPS_BUILD_BACKEND)
    add_subdirectory(backend)
endif()

if(DEVOPS_BUILD_APP)
    add_subdirectory(app)
endif()
