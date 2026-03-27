cmake_minimum_required(VERSION 3.25)

project(snip LANGUAGES CXX)

# Set C++23 with GNU extensions (like -std=gnu++23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Enable compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directory
include_directories(include)

# Collect source and header files
file(GLOB_RECURSE SRC "src/*.cpp")
file(GLOB_RECURSE HDR "src/*.hpp")

# Define executable
add_executable(${PROJECT_NAME} ${SRC} ${HDR})

install(TARGETS snip RUNTIME DESTINATION bin)