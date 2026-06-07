add_library(parity_tree_sitter STATIC
  TreeSitter.cpp
)

target_compile_options(parity_tree_sitter PUBLIC -I${TREE_SITTER_INCLUDE_DIRS})

target_include_directories(parity_tree_sitter
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(parity_tree_sitter
 PUBLIC
  PkgConfig::TREE_SITTER
  parity_theme
  parity_log
  Qt6::Core
  Qt6::Gui
)