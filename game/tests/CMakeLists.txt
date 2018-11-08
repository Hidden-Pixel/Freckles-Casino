cmake_minimum_required(VERSION 3.12)
project(poker-tests)

add_subdirectory(external/googletest)

add_executable(poker-tests "")
target_include_directories(poker-tests PRIVATE ../src/include)
target_sources(poker-tests PRIVATE
        test-main.cpp
        poker-tests.cpp
        ../src/poker.c)

set_source_files_properties(../src/poker.c PROPERTIES LANGUAGE CXX)

target_link_libraries(poker-tests PRIVATE gtest)

