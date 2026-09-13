cmake_minimum_required(VERSION 3.21)
project(devops-console-app LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick)
find_package(devops-core REQUIRED)
find_package(devops-network REQUIRED)
find_package(devops-ui-components REQUIRED)

qt_standard_project_setup()

qt_add_executable(devops-console-app
    src/main.cpp
)

qt_add_qml_module(devops-console-app
    URI DevopsApp
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/views/DashboardView.qml
)

target_link_libraries(devops-console-app
    PRIVATE
        Qt6::Core
        Qt6::Qml
        Qt6::Quick
        devops::core
        devops::network
        devops::ui-components
)

target_compile_features(devops-console-app PRIVATE cxx_std_20)
