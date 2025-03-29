cmake_minimum_required(VERSION 3.2)

file(GLOB_RECURSE SOURCE_FILES "*.c*")

add_executable(test1 test1.cc)
#target_link_libraries(test1 )

add_executable(test2 test2.cc)