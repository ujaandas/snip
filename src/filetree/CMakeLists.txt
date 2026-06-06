add_library(parity_filetree
  FileTree.cpp
  FileSortProxy.cpp
)

target_include_directories(parity_filetree
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(parity_filetree
  PUBLIC
    parity_log
    Qt6::Core
    Qt6::Quick
)
