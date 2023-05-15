cmake_minimum_required(VERSION 3.10)
project(shortest-path)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# # Add the generated Thrift files to the project

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
set(CACHED_BFS_PROCESSOR_SOURCES CachedBFSProcessor.cpp)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(bin)