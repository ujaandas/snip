add_subdirectory(core)

add_executable(snip
  main.cpp
)

target_link_libraries(snip
  PRIVATE
    snip_core
    Qt6::Core
)

install(TARGETS snip
  RUNTIME DESTINATION bin
)
