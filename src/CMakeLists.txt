add_subdirectory(log)
add_subdirectory(editor)
add_subdirectory(filetree)

add_executable(snip
  main.cpp
  qml.qrc
)

target_link_libraries(snip
  PRIVATE
    snip_editor
    snip_filetree
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
)

install(TARGETS snip
  RUNTIME DESTINATION bin
)
