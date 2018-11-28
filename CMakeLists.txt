cmake_minimum_required(VERSION 3.12)
project(Freckles_Casino)

set_property(GLOBAL PROPERTY RUN_TESTS "OFF")

add_subdirectory(game)

get_property(BUILD_TESTS GLOBAL PROPERTY RUN_TESTS)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif(BUILD_TESTS)


