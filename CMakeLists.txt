cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

set(NAME_PR 3DEdit)

project(${NAME_PR})

add_executable(${NAME_PR} src/main.cpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${NAME_PR} glfw)

add_subdirectory(external/glad)
target_link_libraries(${NAME_PR} glad)
