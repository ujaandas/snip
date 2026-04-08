add_library(snip-render STATIC
  renderer.cpp
  translator.cpp
)

snip_apply_target_defaults(snip-render)

target_link_libraries(snip-render
  PUBLIC
    snip-editor
)

add_library(snip::render ALIAS snip-render)
