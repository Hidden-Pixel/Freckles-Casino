cmake_minimum_required(VERSION 3.12)
project(Freckles_Casino_Game)

get_property(BUILD_AS_LIB GLOBAL PROPERTY RUN_TESTS)

if (BUILD_AS_LIB)
    add_library(Freckles_Casino STATIC "")
    set_source_files_properties(src/poker.cpp src/game-screen.cpp src/buffer.cpp PROPERTIES LANGUAGE CXX)
    target_compile_features(Freckles_Casino PUBLIC cxx_std_14)
else()
    add_executable(Freckles_Casino "")
    target_compile_features(Freckles_Casino PUBLIC cxx_std_14)
endif()

IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_definitions(Freckles_Casino PUBLIC DEBUG)
ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")

add_subdirectory(src/external/raylib)

if (WIN32)
    if(MINGW)
        find_package(OpenAL REQUIRED)
        find_package(glfw3 REQUIRED)
        target_compile_definitions(Freckles_Casino PRIVATE _DEFAULT_SOURCE USE_GLFW=3)
        target_link_libraries(Freckles_Casino raylib glfw OpenAL)
    elseif(MSVC)
        add_library(glfw3 SHARED IMPORTED)
        add_library(OpenAL SHARED IMPORTED)

        set_target_properties(glfw3 PROPERTIES
                IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/win32/msvc/glfw3.dll"
                INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/src/include/GLFW")
        set_target_properties(OpenAL PROPERTIES
                IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/win32/msvc/OpenAL32.dll"
                INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/src/include/AL")
        target_link_libraries(Freckles_Casino glfw raylib OpenAL)
    endif()
elseif(APPLE)
    target_compile_definitions(Freckles_Casino PRIVATE _DEFAULT_SOURCE USE_GLFW=3)
    target_compile_options(Freckles_Casino PRIVATE
            -Wall -Wextra -Wpedantic -Wno-missing-braces -Wno-missing-field-initializers)
    target_link_libraries(Freckles_Casino PUBLIC raylib)

    # if NOT debug
    # Os optimizes for cache efficiency
    # target_compile_options(Freckles_Casino PRIVATE -O1 -Os)
endif()

add_custom_command(
        TARGET Freckles_Casino POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets)

target_sources(Freckles_Casino PRIVATE
        src/buffer.cpp
        src/game-screen.cpp
        src/main.cpp
        src/poker.cpp
        src/scene.cpp
        src/sound.cpp
        src/sprite-animation.cpp
        src/input.cpp
        src/ai-behavior.cpp
        )
target_include_directories(Freckles_Casino PUBLIC src/include)
