add_executable(text_tests
  buffer_test.cpp
)

target_link_libraries(text_tests
  PRIVATE
    snip_text
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(text_tests)
