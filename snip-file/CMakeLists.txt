add_library(snip-file STATIC
  file.cpp
)

snip_apply_target_defaults(snip-file)

target_link_libraries(snip-file
  PUBLIC
    snip-core
)

add_library(snip::file ALIAS snip-file)
