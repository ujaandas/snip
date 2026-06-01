add_library(snip_log INTERFACE)

target_include_directories(snip_log
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(snip_log
  INTERFACE
    Qt6::Core
    Qt6::Quick
)
