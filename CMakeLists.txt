cmake_minimum_required(VERSION 3.26.3)
project(adventOfCode2024)

# Set the C++ standard
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the output directories for binaries and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

# Gather all source files for project
file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(adventOfCode2024 ${SOURCES})

# name of the executable
set_target_properties(adventOfCode2024 PROPERTIES OUTPUT_NAME "aoc2024")

# include directories
target_include_directories(adventOfCode2024 PRIVATE ${CMAKE_SOURCE_DIR})

# Precompiled headers
target_precompile_headers(adventOfCode2024 PRIVATE ${CMAKE_SOURCE_DIR}/pch.h)
