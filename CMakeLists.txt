cmake_minimum_required(VERSION 3.5)

project(pwf2tools-execs LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out/bin)

add_subdirectory(execs)

option(BUILD_QTGUI "Build the Qt GUI along with the executables" ON)

if(BUILD_QTGUI)
add_subdirectory(QtGUI)
endif(BUILD_QTGUI)
