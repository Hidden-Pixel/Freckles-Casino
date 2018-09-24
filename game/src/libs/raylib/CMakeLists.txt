cmake_minimum_required(VERSION 3.12)
project(Internal_Raylib)

target_compile_features(Freckles_Casino PRIVATE c_std_11)

add_library(raylib "")

target_sources(raylib PRIVATE
        audio.c
        core.c
        models.c
        rglfw.c
        shapes.c
        text.c
        textures.c
        utils.c
        external/mini_al.c)