add_library(snip-core STATIC
  event_loop.cpp
  event_source.cpp
  ezpipe.cpp
  thread_pool.cpp
)

target_include_directories(snip-core
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(snip-core PUBLIC cxx_std_23)

add_library(snip::core ALIAS snip-core)
