project(FRAMEWORK_UTIL_TEST)

add_executable(${PROJECT_NAME}
    test_main.cpp
    test_Files.cpp
)

target_link_libraries(${PROJECT_NAME}
    FRAMEWORK_UTIL
    doctest::doctest
)

doctest_discover_tests(${PROJECT_NAME})