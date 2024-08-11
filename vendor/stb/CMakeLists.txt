cmake_minimum_required(VERSION 3.28)

project(stb)

add_library(stb)
target_sources(stb PRIVATE src/stb_image_impl.c)
target_include_directories(stb PRIVATE include)
