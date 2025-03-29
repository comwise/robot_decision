cmake_minimum_required(VERSION 3.2) 

project(robot_decision)

set(CMAKE_CXX_FLAGS "-g -std=c++14")

include_directories(${PROJECT_SOURCE_DIR}/include/robot_decision)
include_directories(${PROJECT_SOURCE_DIR}/src)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

link_directories(${PROJECT_SOURCE_DIR}/lib)

#enable_testing()
add_subdirectory(src)