# PATTERN FOR ALL .h an .cpp files
file(GLOB ALL_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../templates/*.h"
)
# CREATE LIBRARY
add_library(
    CLI_TOOL
    boilr.cpp
)

target_include_directories(
    CLI_TOOL
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)