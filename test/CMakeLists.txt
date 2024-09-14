# test/CMakeLists.txt

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/DataStructures)
include_directories(${CMAKE_SOURCE_DIR}/googletest)


# Add the integration directory
add_subdirectory(integration)

# Addd the unit tests directory
add_subdirectory(unit)


