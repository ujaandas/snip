add_library(snip_editor
  Editor.cpp
)

target_include_directories(snip_editor
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(snip_editor
  PUBLIC
    snip_log
    Qt6::Core
    Qt6::Quick
)
