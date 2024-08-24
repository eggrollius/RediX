# test/unit/CMakeLists.txt

add_library(DatabaseTest STATIC
    DatabaseTest.cpp
)

# Create your executable
add_executable(unit_tests unit_tests.cpp)

# Link Google Test libraries
target_link_libraries(unit_tests gtest gtest_main DatabaseTest)
target_link_libraries(unit_tests database)

# Add a custom target for running the integration tests
add_custom_target(run_unit_tests
    COMMAND unit_tests
    DEPENDS unit_tests
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
