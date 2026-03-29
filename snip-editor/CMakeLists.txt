add_library(snip-editor STATIC
  editor.cpp
  text/splitline.cpp
  motion.cpp
)

snip_apply_target_defaults(snip-editor)

target_link_libraries(snip-editor
  PUBLIC
    snip-runtime
)

add_library(snip::editor ALIAS snip-editor)