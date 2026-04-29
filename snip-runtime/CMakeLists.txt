add_library(snip-runtime STATIC
  app.cpp
  cmd.cpp
  runtime.cpp
  input.cpp
)

snip_apply_target_defaults(snip-runtime)

target_link_libraries(snip-runtime
  PUBLIC
    snip-core
  PRIVATE
    snip-file
)

add_library(snip::runtime ALIAS snip-runtime)
