cmake_minimum_required(VERSION 3.25)

project(snip LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Enable compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src/event_loop)
add_subdirectory(src/framework)
add_subdirectory(src/editor)

add_executable(snip-app src/main.cpp)
target_link_libraries(snip-app PRIVATE snip)
set_target_properties(snip-app PROPERTIES OUTPUT_NAME snip)

install(TARGETS snip-app RUNTIME DESTINATION bin)