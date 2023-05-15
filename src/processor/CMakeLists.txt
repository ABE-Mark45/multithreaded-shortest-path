set(CACHED_BFS_PROCESSOR_SOURCES CachedBFSProcessor.cpp)
add_library(CachedBFSProcessor STATIC ${CACHED_BFS_PROCESSOR_SOURCES})
target_link_libraries(CachedBFSProcessor PUBLIC Graph)
target_include_directories(CachedBFSProcessor PRIVATE ${PROJECT_SOURCE_DIR}/src)


set(BFS_PROCESSOR_SOURCES BFSProcessor.cpp)
add_library(BFSProcessor STATIC ${BFS_PROCESSOR_SOURCES})
target_link_libraries(BFSProcessor PUBLIC Graph)
target_include_directories(BFSProcessor PRIVATE ${PROJECT_SOURCE_DIR}/src)
