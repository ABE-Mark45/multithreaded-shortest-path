set(THRIFT_SOURCES GraphService.cpp graph_types.cpp)
add_library(Thrift STATIC ${THRIFT_SOURCES})
target_include_directories(Thrift PRIVATE ${PROJECT_SOURCE_DIR}/src)