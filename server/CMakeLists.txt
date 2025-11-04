cmake_minimum_required(VERSION 3.14)
project(TCPServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)

# Include directories
include_directories(${SODIUM_INCLUDE_DIRS})

# Source files
set(SOURCES
    server.cpp
    client.cpp
    crypto.cpp
)

# Main executables
add_executable(server ${SOURCES} test/main_server.cpp)
add_executable(client ${SOURCES} test/main_client.cpp)

# Link libraries
target_link_libraries(server ${SODIUM_LIBRARIES} pthread)
target_link_libraries(client ${SODIUM_LIBRARIES} pthread)

# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Test executable
add_executable(
  tests
  ${SOURCES}
  test/test_main.cpp
  test/test_crypto.cpp
  test/test_client.cpp
  test/test_server.cpp
)

target_link_libraries(
  tests
  ${GTEST_LIBRARIES}
  ${GTEST_MAIN_LIBRARIES}
  ${SODIUM_LIBRARIES}
  pthread
)

# Enable testing
enable_testing()
add_test(NAME unit_tests COMMAND tests)