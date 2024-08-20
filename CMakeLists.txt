cmake_minimum_required(VERSION 3.11)

# Project name
project(Redix)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler flags
add_compile_options(-Wall -Wextra)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

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
target_link_libraries(redix pthread)

# Create a library from client.cpp
add_library(client_lib STATIC src/client.cpp)

# Add the server library
add_library(server STATIC
    src/server.cpp
    src/database.cpp
    src/ClientConnection.cpp
    src/RequestHandler.cpp
    src/OperationTypes.cpp
)

# Add the test directory
add_subdirectory(test)

# Custom commands
add_custom_target(run
    COMMAND ./redix
    DEPENDS redix
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
