cmake_minimum_required(VERSION 3.21)
project(devops-ui-components LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick)
find_package(devops-core REQUIRED)
find_package(devops-network REQUIRED)

qt_standard_project_setup()

qt_add_library(devops-ui-components STATIC
    src/services_view_model.cpp
)
add_library(devops::ui-components ALIAS devops-ui-components)

# Module QML : les .qml sont compilés (qmlcachegen) et exposés sous
# l'URI DevopsUi, importable depuis app/ avec `import DevopsUi`.
qt_add_qml_module(devops-ui-components
    URI DevopsUi
    VERSION 1.0
    QML_FILES
        qml/StatusBadge.qml
        qml/ServiceCard.qml
)

target_include_directories(devops-ui-components
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(devops-ui-components
    PUBLIC
        Qt6::Core
        Qt6::Qml
        Qt6::Quick
        devops::core
        devops::network
)

target_compile_features(devops-ui-components PUBLIC cxx_std_20)
