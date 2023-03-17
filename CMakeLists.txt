cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

set(NAME_PR 3DEdit)

project(${NAME_PR})

add_executable(${NAME_PR} 
	src/main.cpp
	src/Renderer/ShaderProgram.cpp
	src/Renderer/ShaderProgram.h
	src/Resources/ResourceManager.cpp
	src/Resources/ResourceManager.h
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${NAME_PR} glfw)

add_subdirectory(external/glad)
target_link_libraries(${NAME_PR} glad)

set_target_properties(${NAME_PR} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${NAME_PR} POST_BUILD 
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${NAME_PR}>/res)
