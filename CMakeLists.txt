# VERSION
cmake_minimum_required(VERSION 3.15)

# PROJECT (.exe name)
project(br VERSION 1.0.0 LANGUAGES CXX)

# STANDARD C++ NEEDED
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cross-platform settings
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Build type (default to Release if not specified)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ADD MAIN.cpp
add_executable(${PROJECT_NAME} main.cpp)

# SUBDIRECTORIES
add_subdirectory(include)

# INCLUDE LIBRARIES
target_link_libraries(${PROJECT_NAME} PUBLIC CLI_TOOL)

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "br"
        SUFFIX ".exe"
    )
elseif(APPLE)
    # macOS-specific settings
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "br"
    )
elseif(UNIX)
    # Linux-specific settings
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "br"
    )
endif()

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
