add_subdirectory(text)
add_subdirectory(log)
add_subdirectory(viewport)

add_executable(snip
  main.cpp
  qml.qrc
)

target_link_libraries(snip
  PRIVATE
    snip_text
    snip_viewport
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
)

install(TARGETS snip
  RUNTIME DESTINATION bin
)
