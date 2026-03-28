add_library(snip-runtime STATIC
  cmd.cpp
  input.cpp
  file/file.cpp
  terminal/terminal.cpp
)

snip_apply_target_defaults(snip-runtime)

target_link_libraries(snip-runtime
  PUBLIC
    snip-core
)

add_library(snip::runtime ALIAS snip-runtime)
