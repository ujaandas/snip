add_library(
  parity_tree_sitter STATIC
  TreeSitter.cpp
)

target_include_directories(parity_tree_sitter
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
  parity_tree_sitter
  PUBLIC
  parity_log
  Qt6::Core
  Qt6::Gui
  PkgConfig::TREE_SITTER
)
