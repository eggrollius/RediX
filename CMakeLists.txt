cmake_minimum_required(VERSION 3.11)
project(Redix)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra)

# Add Google Test
enable_testing()
add_subdirectory(googletest)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/DataStructures)

# Create libraries
add_library(server STATIC
    src/server.cpp
    src/ResponseMessage.cpp
    src/database.cpp
    src/ClientConnection.cpp
    src/RequestHandler.cpp
    src/OperationTypes.cpp
)

add_library(client STATIC src/client.cpp)

add_library(database STATIC 
    src/database.cpp   
    src/ResponseMessage.cpp 
)

add_library(datastructures STATIC
    src/DataStructures/LinkedList.cpp
    src/DataStructures/Node.cpp
    src/DataStructures/SortedSet.cpp
)

# Create the executable for cli
add_executable(redixcli src/rediXCLI.cpp)

# Create the executable for server
add_executable(redix src/rediXServer.cpp)

# Link libraries to the executable
target_link_libraries(redixcli PRIVATE server client datastructures Threads::Threads)
target_link_libraries(redix PRIVATE server)

# Link libraries to other libraries as needed
target_link_libraries(server PRIVATE datastructures)
target_link_libraries(client PRIVATE datastructures)

# Find and link pthread
find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE Threads::Threads)
target_link_libraries(client PRIVATE Threads::Threads)

# Add the test directory
add_subdirectory(test)
