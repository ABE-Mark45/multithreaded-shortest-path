set(LOGGER_SOURCES Logger.cpp)
add_library(Logger STATIC ${LOGGER_SOURCES})
target_include_directories(Logger PRIVATE ${PROJECT_SOURCE_DIR}/src)