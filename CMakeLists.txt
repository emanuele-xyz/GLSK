cmake_minimum_required(VERSION 3.28)

project(
    glsk
    VERSION 1.0
    LANGUAGES C
)

# Compile static libraries
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# For MSVC, make sure that the runtime library is statically linked
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Configure GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/glad)
add_subdirectory(vendor/glfw-3.4)
add_subdirectory(vendor/stb)

add_executable("${CMAKE_PROJECT_NAME}")
set_target_properties(
    "${CMAKE_PROJECT_NAME}" 
    PROPERTIES
    C_STANDARD 23
)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
target_sources(
    "${CMAKE_PROJECT_NAME}" 
    PRIVATE 
    ${SOURCES}
)
target_include_directories(
    "${CMAKE_PROJECT_NAME}"
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw-3.4/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb"
     "${CMAKE_CURRENT_SOURCE_DIR}/vendor/cglm-0.9.4/include"
)
target_link_libraries(
    "${CMAKE_PROJECT_NAME}"
    PRIVATE
    glad
    glfw
    stb
)
target_compile_definitions(
    "${CMAKE_PROJECT_NAME}"
    PRIVATE
    $<$<CONFIG:Debug>:GLSK_DEBUG>
    $<$<CONFIG:Release>:GLSK_RELEASE>
)
if (MSVC)
    target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE /Wall)
else()
    target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE -Wall -Wextra -Wpedantic)
endif()
