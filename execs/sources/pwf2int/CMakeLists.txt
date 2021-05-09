cmake_minimum_required(VERSION 3.5)

project(pwf2tools-execs LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../../out/bin)

set(PROJECT_SOURCES 
    int.cpp
    pwf2int.cpp
)

if(WIN32)
    add_executable(pwf2int
        lzss.c
        res/pwf2int.rc
        ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_SOURCES}
    )
else()
    add_executable(pwf2int
        lzss.c
        ${PROJECT_SOURCES}
    )
endif()
