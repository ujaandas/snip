add_library(snip-runtime STATIC
  cmd.cpp
  input.cpp
  loop_wiring.cpp
  file/file.cpp
  terminal/terminal.cpp
)

target_include_directories(snip-runtime
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(snip-runtime
  PUBLIC
    snip-core
)

target_compile_features(snip-runtime PUBLIC cxx_std_23)

add_library(snip::runtime ALIAS snip-runtime)
