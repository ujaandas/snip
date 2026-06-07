add_subdirectory(log)
add_subdirectory(theme)
add_subdirectory(editor)
add_subdirectory(filetree)
add_subdirectory(tabs)
add_subdirectory(lsp)
add_subdirectory(tree-sitter)

add_executable(parity
  main.cpp
  qml.qrc
)

target_link_libraries(parity
  PRIVATE
    parity_theme
    parity_editor
    parity_filetree
    parity_tabs
    parity_lsp
    parity_tree_sitter
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
)

install(TARGETS parity
  RUNTIME DESTINATION bin
)
