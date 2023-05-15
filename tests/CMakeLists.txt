enable_testing()
include(GoogleTest)


add_executable(
  graph_test
  graph_test.cpp
)
  
target_include_directories(graph_test PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(
  graph_test
  GTest::gtest_main
  Graph
  BFSProcessor
  CachedBFSProcessor
  CachedBFSCoordinator
  AllPairsCoordinator
  Thrift
)

gtest_discover_tests(graph_test)