set(GRAPH_SOURCES graph.cpp)
add_library(Graph STATIC ${GRAPH_SOURCES})
target_include_directories(Graph PRIVATE ${PROJECT_SOURCE_DIR}/src)
