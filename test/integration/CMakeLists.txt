# test/integration/CMakeLists.txt

# Source files for integration tests
set(INTEGRATION_TEST_SOURCES
    integration_test.cpp 
)

# Define the target
add_executable(integration_tests
    ${INTEGRATION_TEST_SOURCES}
)

# Link libraries and client_lib + server library
target_link_libraries(integration_tests 
    PRIVATE
        pthread 
        client_lib 
        server 
)

# Add a custom target for running the integration tests
add_custom_target(run_integration_tests
    COMMAND integration_tests
    DEPENDS integration_tests
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
