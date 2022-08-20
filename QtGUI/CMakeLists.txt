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

option(ENABLE_DISCORDRPC "Enable Discord Rich Presence" ON)
if(ENABLE_DISCORDRPC)
    option(BUILD_EXAMPLES "" OFF)
    add_subdirectory(discord-rpc)
    add_compile_definitions(ENABLE_RICHPRESENCE)
endif()

set(PROJECT_SOURCES
        main.cpp
        pwf2about.cpp
        pwf2about.h
        pwf2about.ui
        pwf2debug.cpp
        pwf2debug.h
        pwf2debug.ui
        pwf2discord.cpp
        pwf2discord.h
        pwf2hat.cpp
        pwf2hat.h
        pwf2hat.ui
        pwf2info.cpp
        pwf2info.h
        pwf2int.cpp
        pwf2int.h
        pwf2int.ui
        pwf2log.cpp
        pwf2log.h
        pwf2main.cpp
        pwf2main.h
        pwf2main.qrc
        pwf2main.ui
        pwf2options.cpp
        pwf2options.h
        pwf2options.ui
        pwf2spm.cpp
        pwf2spm.h
        pwf2spm.ui
        pwf2tex.cpp
        pwf2tex.h
        pwf2tex.ui
)

if(WIN32)
    if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        qt_add_executable(pwf2tools
            ${PROJECT_SOURCES}
            res/pwf2tools.rc
        )
    else()
        add_executable(pwf2tools
            ${PROJECT_SOURCES}
            res/pwf2tools.rc
        )
    endif()
else()
    if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        qt_add_executable(pwf2tools
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(pwf2tools
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if(ENABLE_DISCORDRPC)
    target_link_libraries(pwf2tools PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        discord-rpc
    )
else()
    target_link_libraries(pwf2tools PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
    )
endif()
