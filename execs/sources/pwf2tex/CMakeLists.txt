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
    gs.h
    pnghelper.cpp
    pnghelper.h
    pwf2cmd.h
    pwf2common.h
    pwf2tex.cpp
    tim2.cpp
    tim2.h
    tim2upload.cpp
    tim2upload.h
    types.h
)

if(WIN32)
    add_executable(pwf2tex
        ${PROJECT_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/res/pwf2tex.rc
    )
else()
    add_executable(pwf2tex
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(pwf2tex ZLIB::ZLIB PNG::PNG)
