add_library(snip_tabs
  TabManager.cpp
)

target_include_directories(snip_tabs
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(snip_tabs
  PUBLIC
    snip_log
    Qt6::Core
    Qt6::Quick
)
