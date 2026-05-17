add_library(snip_io
  OpenFile.cpp
)

target_include_directories(snip_io
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(snip_io
  PUBLIC
    snip_text
    Qt6::Core
)
