cmake_minimum_required(VERSION 3.16)
project(snip LANGUAGES CXX)

# Use gnu++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)

# Find headers in include/
include_directories(include)

# Get all src files
file(GLOB SRC_FILES src/*.cpp)

# Build
add_executable(snip ${SRC_FILES})
