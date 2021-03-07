cmake_minimum_required(VERSION 3.5)

project(pwf2tools-execs LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../../out/bin)

find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)
if(NOT PNG_FOUND)
    message(FATAL_ERROR "libpng is not installed!")
endif()

if(NOT ZLIB_FOUND)
    message(FATAL_ERROR "zlib is not installed!")
endif()

set(PROJECT_SOURCES
    gs.cpp
    pnghelper.cpp
    pwf2tex.cpp
    tim2.cpp
    tim2upload.cpp
)

add_executable(pwf2tex
            ${PROJECT_SOURCES}
)

target_link_libraries(pwf2tex ZLIB::ZLIB PNG::PNG)
