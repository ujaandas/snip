add_library(parity_lsp
  LspClient.cpp
  SemanticTokens.cpp
)

target_include_directories(parity_lsp
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(parity_lsp
  PUBLIC
    parity_theme
    parity_log
    Qt6::Core
    Qt6::Network
)
