cmake_minimum_required(VERSION 3.2)

file(GLOB_RECURSE SOURCE_FILES "*.c*")

add_executable(test_decision test_decision.cc)
#target_link_libraries(test_decision )

add_executable(test2 test2.cc)

add_executable(test_scheduler test_scheduler.cc)