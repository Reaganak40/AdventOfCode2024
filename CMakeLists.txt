cmake_minimum_required(VERSION 3.28.3)
project(aventOfCode2024)

# Set the C++ standard
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the output directories for binaries and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

# Gather all source files for project
file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/day1/*.cpp
)

add_executable(aventOfCode2024 ${SOURCES})

# name of the executable
set_target_properties(aventOfCode2024 PROPERTIES OUTPUT_NAME "aoc2024")

# include directories
target_include_directories(aventOfCode2024 PRIVATE ${CMAKE_SOURCE_DIR})

# Precompiled headers
target_precompile_headers(aventOfCode2024 PRIVATE ${CMAKE_SOURCE_DIR}/pch.h)
