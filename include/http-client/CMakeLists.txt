# PATTERN FOR ALL .h an .cpp files
file(GLOB ALL_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/registry-services/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/registry-services/*.h"
)
# CREATE LIBRARY
add_library(
    HTTP_CLIENT
    STATIC 
    ${ALL_SOURCES}
)

find_package(CURL REQUIRED)
target_link_libraries(HTTP_CLIENT PUBLIC CURL::libcurl)

target_include_directories(
    HTTP_CLIENT
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)