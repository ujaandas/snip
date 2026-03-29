add_library(snip-term STATIC
  terminal.cpp
)

snip_apply_target_defaults(snip-term)

target_link_libraries(snip-term
  PUBLIC
    snip-core
)

add_library(snip::term ALIAS snip-term)
