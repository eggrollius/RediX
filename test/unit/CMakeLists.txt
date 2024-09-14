# test/unit/CMakeLists.txt

add_library(DatabaseTest STATIC
    DatabaseTest.cpp
)

target_link_libraries(DatabaseTest PRIVATE
    gtest
    gtest_main
    database
)

add_library(LinkedListTest STATIC
    LinkedListTest.cpp
)

add_library(SortedSetTest STATIC
    SortedSetTest.cpp
)

target_link_libraries(LinkedListTest PRIVATE
    gtest
    gtest_main
    datastructures
)

target_link_libraries(SortedSetTest PRIVATE
    gtest
    gtest_main
    datastructures
)

# Create your executable
add_executable(unit_tests unit_tests.cpp)

# Link Google Test libraries
target_link_libraries(unit_tests gtest gtest_main DatabaseTest LinkedListTest SortedSetTest)

# Add a custom target for running the integration tests
add_custom_target(run_unit_tests
    COMMAND unit_tests
    DEPENDS unit_tests
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
