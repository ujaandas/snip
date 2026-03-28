add_library(snip-editor STATIC
  editor.cpp
  text/splitline.cpp
)

target_include_directories(snip-editor
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(snip-editor
  PUBLIC
    snip-runtime
)

target_compile_features(snip-editor PUBLIC cxx_std_23)

add_library(snip::editor ALIAS snip-editor)