add_library(snip-ui STATIC
  renderer.cpp
)

snip_apply_target_defaults(snip-ui)

target_link_libraries(snip-ui
  PUBLIC
    snip-editor
)

add_library(snip::ui ALIAS snip-ui)