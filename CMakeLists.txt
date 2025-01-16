# Minimum required CMake version
cmake_minimum_required(VERSION 3.15)

# Project name and C++ standard
project(HTTP_Server_CPP LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directories for source, headers, and build output
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(BUILD_DIR ${CMAKE_BINARY_DIR})

# Output executable name
set(OUTPUT App)

# Include header files
include_directories(${INCLUDE_DIR})

# Source files
set(SOURCES
    ${SRC_DIR}/Server.cpp
    ${SRC_DIR}/Main.cpp
)

# Add executable
add_executable(${OUTPUT} ${SOURCES})

# Set build output directory
set_target_properties(${OUTPUT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

# Message on successful configuration
message("CMake configuration complete. Build output will be placed in: ${BUILD_DIR}")
