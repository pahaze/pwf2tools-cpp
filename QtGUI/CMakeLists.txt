cmake_minimum_required(VERSION 3.5)

project(pwf2tools LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../out)

set(PROJECT_SOURCES
        main.cpp
        pwf2about.cpp
        pwf2about.h
        pwf2about.ui
        pwf2debug.cpp
        pwf2debug.h
        pwf2debug.ui
        pwf2hat.cpp
        pwf2hat.h
        pwf2hat.ui
        pwf2int.cpp
        pwf2int.h
        pwf2int.ui
        pwf2main.cpp
        pwf2main.h
        pwf2main.qrc
        pwf2main.ui
        pwf2options.cpp
        pwf2options.h
        pwf2options.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(pwf2tools
        ${PROJECT_SOURCES}
    )
else()
        add_executable(pwf2tools
            ${PROJECT_SOURCES}
        )
endif()

target_link_libraries(pwf2tools PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
