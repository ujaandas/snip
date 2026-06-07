cmake_minimum_required(VERSION 3.20)

project(parity LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

enable_testing()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2)
find_package(PkgConfig REQUIRED)

pkg_check_modules(TREE_SITTER REQUIRED IMPORTED_TARGET tree-sitter)

add_subdirectory(src)
add_subdirectory(tests)
