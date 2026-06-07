add_library(
  parity_tree_sitter STATIC
  TreeSitter.cpp
)

target_include_directories(
  parity_tree_sitter
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src/log
)

target_link_libraries(
  parity_tree_sitter
  PUBLIC
  Qt6::Core
  Qt6::Gui
  ${CMAKE_DL_LIBS}
)
