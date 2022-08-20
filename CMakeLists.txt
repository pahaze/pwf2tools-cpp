cmake_minimum_required(VERSION 3.5)

project(pwf2tools LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out/bin)

option(BUILD_EXECS "Build the executables" ON)

if(BUILD_EXECS)
    add_subdirectory(execs)
endif(BUILD_EXECS)

option(BUILD_QTGUI "Build the Qt GUI" ON)

if(BUILD_QTGUI)
    add_subdirectory(QtGUI)
endif(BUILD_QTGUI)

option(ENABLE_DISCORDRPC "Enable Discord Rich Presence" ON)
