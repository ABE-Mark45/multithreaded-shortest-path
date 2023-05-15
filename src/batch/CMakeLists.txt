set(BATCH_SOURCES Batch.cpp)
add_library(Batch STATIC ${BATCH_SOURCES})
target_link_libraries(Batch PRIVATE Thrift)
target_include_directories(Batch PRIVATE ${PROJECT_SOURCE_DIR}/src)
