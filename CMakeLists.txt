cmake_minimum_required(VERSION 3.10)

# Project name
project(Redix)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler flags
add_compile_options(-Wall -Wextra)

# Include directories
include_directories(/usr/local/include)

# Link directories
link_directories(/usr/local/lib)

# Source files in the src directory
set(SOURCE_FILES
    src/main.cpp
    src/server.cpp
    src/database.cpp
    src/ClientConnection.cpp
    src/RequestHandler.cpp
    src/OperationTypes.cpp
)

# Create the executable
add_executable(redix ${SOURCE_FILES})

# Link libraries
target_link_libraries(redix gtest gtest_main pthread)

# Custom commands
add_custom_target(run
    COMMAND ./redix
    DEPENDS redix
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
