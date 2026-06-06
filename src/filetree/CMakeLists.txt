add_library(snip_filetree
  FileTreeModel.cpp
  FileTreeController.cpp
)

target_include_directories(snip_filetree
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(snip_filetree
  PUBLIC
    snip_log
    Qt6::Core
    Qt6::Quick
)
