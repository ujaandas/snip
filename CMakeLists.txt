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

set(SNIP_PUBLIC_INCLUDE_DIR ${PROJECT_SOURCE_DIR})

function(snip_apply_target_defaults target)
	target_include_directories(${target}
		PUBLIC
			$<BUILD_INTERFACE:${SNIP_PUBLIC_INCLUDE_DIR}>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	)
	target_compile_features(${target} PUBLIC cxx_std_23)
endfunction()

add_subdirectory(snip-core)
add_subdirectory(snip-file)
add_subdirectory(snip-term)
add_subdirectory(snip-runtime)
add_subdirectory(snip-editor)
add_subdirectory(snip-render)
add_subdirectory(snip-ui)

add_executable(snip-app snip/main.cpp)
target_link_libraries(snip-app PRIVATE snip-runtime snip-term snip-editor snip-ui)
set_target_properties(snip-app PROPERTIES OUTPUT_NAME snip)

install(
	TARGETS snip-core snip-file snip-term snip-runtime snip-editor snip-render snip-ui snip-app
	EXPORT snipTargets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
	DIRECTORY snip-core snip-file snip-term snip-runtime snip-editor snip-render snip-ui
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILES_MATCHING PATTERN "*.hpp"
)

install(
	EXPORT snipTargets
	FILE snipTargets.cmake
	NAMESPACE snip::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/snip
)