add_library(snip_viewport
  StrictScrollViewport.cpp
)

target_include_directories(snip_viewport
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(snip_viewport
  PUBLIC
    snip_log
    Qt6::Core
    Qt6::Quick
)
