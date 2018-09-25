cmake_minimum_required(VERSION 3.12)
project(Freckles_Casino_Game)
add_executable(Freckles_Casino "")
target_compile_features(Freckles_Casino PUBLIC cxx_std_17)

if (WIN32)
    add_library(raylib STATIC IMPORTED)

    if(MINGW)
        find_package(OpenAL REQUIRED)
        find_package(glfw3 REQUIRED)
        set_target_properties(raylib PROPERTIES
                IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/win32/mingw32/libraylib_static.a"
                INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/raylib")

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
        set_target_properties(raylib PROPERTIES
                IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/win32/msvc/raylib.lib"
                INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/src/libs/raylib")
        target_link_libraries(Freckles_Casino glfw raylib OpenAL)
    endif()
elseif(APPLE)
    target_compile_definitions(Freckles_Casino PRIVATE _DEFAULT_SOURCE USE_GLFW=3)
    target_compile_options(Freckles_Casino PRIVATE -fgnu89-inline -Wall -Wno-missing-braces -s)
    target_link_libraries(Freckles_Casino raylib)

    # if NOT debug
    # target_compile_options(Freckles_Casino PRIVATE -O1)
endif()

add_custom_command(
        TARGET Freckles_Casino POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets)

target_sources(Freckles_Casino PRIVATE src/main.c)
set_source_files_properties(src/main.c PROPERTIES LANGUAGE CXX)
target_include_directories(Freckles_Casino PUBLIC src/include)