add_library(parity_editor
  Editor.cpp
  StrictScroll.cpp
)

target_include_directories(parity_editor
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(parity_editor
  PUBLIC
    parity_log
    Qt6::Core
    Qt6::Quick
)
