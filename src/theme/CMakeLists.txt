add_library(parity_theme
  Theme.cpp
)

target_include_directories(parity_theme
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(parity_theme
  PUBLIC
    Qt6::Core
    Qt6::Gui
)
