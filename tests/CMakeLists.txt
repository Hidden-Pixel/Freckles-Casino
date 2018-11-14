cmake_minimum_required(VERSION 3.12)
project(poker-tests)

add_subdirectory(external/googletest)

add_executable(poker-tests "")
target_include_directories(poker-tests PRIVATE ../src/include)
target_sources(poker-tests PRIVATE
        test-main.cpp
        poker-tests.cpp)

target_link_libraries(poker-tests PRIVATE gtest Freckles_Casino)

