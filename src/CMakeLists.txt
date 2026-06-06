add_subdirectory(log)
add_subdirectory(editor)
add_subdirectory(filetree)
add_subdirectory(tabs)

add_executable(parity
  main.cpp
  qml.qrc
)

target_link_libraries(parity
  PRIVATE
    parity_editor
    parity_filetree
    parity_tabs
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
)

install(TARGETS parity
  RUNTIME DESTINATION bin
)
