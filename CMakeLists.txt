cmake_minimum_required(VERSION 3.25)

project(snip LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Enable compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES 
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

add_subdirectory(src/snip-core)
add_subdirectory(src/snip-runtime)
add_subdirectory(src/snip-editor)
add_subdirectory(src/snip-ui)

add_executable(snip-app src/snip/main.cpp)
target_link_libraries(snip-app PRIVATE snip-runtime snip-editor snip-ui)
set_target_properties(snip-app PROPERTIES OUTPUT_NAME snip)

install(
	TARGETS snip-core snip-runtime snip-editor snip-ui snip-app
	EXPORT snipTargets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
	DIRECTORY src/snip-core src/snip-runtime src/snip-editor src/snip-ui
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILES_MATCHING PATTERN "*.hpp"
)

install(
	EXPORT snipTargets
	FILE snipTargets.cmake
	NAMESPACE snip::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/snip
)