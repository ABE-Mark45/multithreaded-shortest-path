set(BFS_COORDINATOR_SOURCES BFSCoordinator.cpp)
add_library(BFSCoordinator STATIC ${BFS_COORDINATOR_SOURCES})
target_link_libraries(BFSCoordinator PRIVATE Thrift Graph BFSProcessor Batch)
target_include_directories(BFSCoordinator PRIVATE ${PROJECT_SOURCE_DIR}/src)


set(CACHED_BFS_COORDINATOR_SOURCES CachedBFSCoordinator.cpp)
add_library(CachedBFSCoordinator STATIC ${CACHED_BFS_COORDINATOR_SOURCES})
target_link_libraries(CachedBFSCoordinator PRIVATE Thrift Graph CachedBFSProcessor ConcurrentQueue Batch)
target_include_directories(CachedBFSCoordinator PRIVATE ${PROJECT_SOURCE_DIR}/src)


set(ALL_PAIRS_COORDINATOR_SOURCES AllPairsCoordinator.cpp)
add_library(AllPairsCoordinator STATIC ${ALL_PAIRS_COORDINATOR_SOURCES})
target_link_libraries(AllPairsCoordinator PRIVATE Graph)
target_include_directories(AllPairsCoordinator PRIVATE ${PROJECT_SOURCE_DIR}/src)
