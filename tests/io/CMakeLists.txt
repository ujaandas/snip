add_executable(io_tests
  open_file_test.cpp
)

target_link_libraries(io_tests
  PRIVATE
    snip_io
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(io_tests)
