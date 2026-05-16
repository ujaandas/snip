add_library(snip_text
  Buffer.cpp
)

target_include_directories(snip_text
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(snip_text
  PUBLIC
    Qt6::Core
)
