cmake_minimum_required(VERSION 3.2)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory("vendor/glad")
add_subdirectory("vendor/stb")
add_subdirectory("vendor/glm")

add_subdirectory("LearnOpenGL")

