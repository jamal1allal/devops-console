cmake_minimum_required(VERSION 3.21)

project(devops-ui-components LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick)
find_package(devops-core REQUIRED)
find_package(devops-network REQUIRED)

# qt_standard_project_setup()
qt_standard_project_setup(REQUIRES 6.5)

qt_add_library(devops-ui-components STATIC)

add_library(devops::ui-components ALIAS devops-ui-components)

target_include_directories(devops-ui-components
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ui-components
)

qt_add_qml_module(devops-ui-components
    URI DevopsUi
    VERSION 1.0

    SOURCES
        include/ui-components/services_view_model.h
        src/services_view_model.cpp

    QML_FILES
        qml/StatusBadge.qml
        qml/ServiceCard.qml
)

target_link_libraries(devops-ui-components
    PUBLIC
        Qt6::Core
        Qt6::Qml
        Qt6::Quick
        devops::core
        devops::network
)

target_compile_features(devops-ui-components
    PUBLIC
        cxx_std_20
)

install(
    TARGETS
        devops-ui-components
        devops-ui-componentsplugin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(
    DIRECTORY include/
    DESTINATION include
)

