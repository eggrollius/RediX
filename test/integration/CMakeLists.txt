# test/integration/CMakeLists.txt

# Create your executable
add_executable(integration_tests integration_test.cpp)

# Link Google Test libraries
target_link_libraries(integration_tests gtest gtest_main server client)

# Add a custom target for running the integration tests
add_custom_target(run_integration_tests
    COMMAND integration_tests
    DEPENDS integration_tests
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
