cmake_minimum_required(VERSION 3.11)
project(Redix)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra)

# Create the server library
add_library(server STATIC
    src/server.cpp
    src/ResponseMessage.cpp
    src/database.cpp
    src/ClientConnection.cpp
    src/RequestHandler.cpp
    src/OperationTypes.cpp
)

# Create the client library
add_library(client_lib STATIC src/client.cpp)

# Create the executable
add_executable(redix src/main.cpp)

# Find and link pthread
find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE Threads::Threads)
target_link_libraries(client_lib PRIVATE Threads::Threads)
target_link_libraries(redix PRIVATE server client_lib Threads::Threads)

# Add the test directory
add_subdirectory(test)

# Custom commands
add_custom_target(run
    COMMAND ./redix
    DEPENDS redix
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
