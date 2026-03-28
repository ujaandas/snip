add_library(snip STATIC
  snip.cpp
  text/splitline.cpp
)

target_include_directories(snip
  PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(snip
  PUBLIC
    snip-runtime
)

target_compile_features(snip PUBLIC cxx_std_23)

add_library(snip::app ALIAS snip)
