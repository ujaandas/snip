add_library(snip-ui STATIC
  renderer.cpp
)

target_include_directories(snip-ui
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(snip-ui
  PUBLIC
    snip-editor
)

target_compile_features(snip-ui PUBLIC cxx_std_23)

add_library(snip::ui ALIAS snip-ui)