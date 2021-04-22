cmake_minimum_required(VERSION 3.5)

project(fit-mqtt-client VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS REQUIRED
    Widgets
)

set(PROJECT_SOURCES
        resources.qrc
        src/main.cpp
        src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui
        src/dashboardpage.cpp src/dashboardpage.h src/dashboardpage.ui
        src/explorerpage.cpp src/explorerpage.h src/explorerpage.ui

)

add_executable(fit-mqtt-client
    ${PROJECT_SOURCES}
)

target_link_libraries(fit-mqtt-client PRIVATE Qt5::Widgets)

set_target_properties(fit-mqtt-client PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)
