cmake_minimum_required(VERSION 3.22)
project(Optics)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)
add_subdirectory(glm)

include_directories(glad/include)

add_executable(Optics main.cpp glad/src/glad.c)

target_link_libraries(Optics glfw)
target_link_libraries(Optics glm)
target_link_libraries(Optics OpenGL::GL)