add_subdirectory(log)
add_subdirectory(editor)

add_executable(snip
  main.cpp
  qml.qrc
)

target_link_libraries(snip
  PRIVATE
    snip_editor
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
)

install(TARGETS snip
  RUNTIME DESTINATION bin
)
