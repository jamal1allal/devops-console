find_package(GTest REQUIRED)

add_executable(devops-core-tests
    test_logger.cpp
    test_event_bus.cpp
)

target_link_libraries(devops-core-tests
    PRIVATE
        devops::core
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(devops-core-tests)
