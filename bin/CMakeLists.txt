add_executable(Server server.cpp)
target_link_libraries(Server PRIVATE 
    Batch
    Graph
    CachedBFSCoordinator
    Dispatcher
    AllPairsCoordinator
    CachedBFSProcessor
    BFSProcessor
    ConcurrentQueue
    Thrift
    thrift
    pthread
)
target_include_directories(Server PRIVATE ${PROJECT_SOURCE_DIR}/src)

add_executable(Client client.cpp)
target_link_libraries(Client PRIVATE 
    Thrift
    thrift
)
target_include_directories(Client PRIVATE ${PROJECT_SOURCE_DIR}/src)
