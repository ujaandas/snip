add_library(parity_tabs
  TabManager.cpp
)

target_include_directories(parity_tabs
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(parity_tabs
  PUBLIC
    parity_log
    parity_editor
    parity_lsp
    Qt6::Core
    Qt6::Quick
)
