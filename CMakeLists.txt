# VERSION
cmake_minimum_required(VERSION 4.0)

# PROJECT (.exe name)
project(br)

# STANDARD C++ NEEDED
set(CMAKE_CXX_STANDARD 17)

# ADD MAIN.cpp
add_executable(${PROJECT_NAME} main.cpp)

# SUBDIRECTORIES
add_subdirectory(include)

# INCLUDE LIBRARIES
target_link_libraries(${PROJECT_NAME} PUBLIC CLI_TOOL)

