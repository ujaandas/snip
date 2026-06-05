add_library(snip_text
  FileBuffer.cpp
  EditorView.cpp
)

target_include_directories(snip_text
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(snip_text
  PUBLIC
    snip_log
    snip_viewport
    Qt6::Core
    Qt6::Quick
)
