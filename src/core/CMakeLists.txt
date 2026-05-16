add_library(snip_core
  Hello.cpp
)

target_include_directories(snip_core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
