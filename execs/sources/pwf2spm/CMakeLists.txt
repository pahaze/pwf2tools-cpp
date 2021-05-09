cmake_minimum_required(VERSION 3.5)

project(pwf2tools-execs LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../../out/bin)

set(PROJECT_SOURCES
    pwf2spm.cpp
    spm.cpp
)

if(WIN32)
     add_executable(pwf2spm
        ${PROJECT_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/res/pwf2spm.rc
    )
else()
    add_executable(pwf2spm
        ${PROJECT_SOURCES}
    )
endif()
