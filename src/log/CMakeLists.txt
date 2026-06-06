add_library(parity_log INTERFACE)

target_include_directories(parity_log
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(parity_log
  INTERFACE
    Qt6::Core
    Qt6::Quick
)
