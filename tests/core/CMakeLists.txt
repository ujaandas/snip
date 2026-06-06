add_executable(core_tests
  hello_test.cpp
)

target_link_libraries(core_tests
  PRIVATE
    parity_core
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(core_tests)
