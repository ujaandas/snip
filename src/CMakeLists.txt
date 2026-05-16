add_subdirectory(core)

add_executable(snip
  main.cpp
)

target_link_libraries(snip
  PRIVATE
    snip_core
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

install(TARGETS snip
  RUNTIME DESTINATION bin
)
