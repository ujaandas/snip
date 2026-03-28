add_library(snip-core STATIC
  event_loop.cpp
  event_source.cpp
  ezpipe.cpp
  thread_pool.cpp
)

snip_apply_target_defaults(snip-core)

add_library(snip::core ALIAS snip-core)
