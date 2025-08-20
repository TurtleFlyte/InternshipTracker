cmake_minimum_required(VERSION 3.27)
project(InternshipTracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)

add_executable(InternshipTracker main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        src/internshiptable.cpp
        include/internshiptable.h
        src/internshipapimanager.cpp
        include/internshipapimanager.h)
target_link_libraries(InternshipTracker
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
)

