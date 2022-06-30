cmake_minimum_required(VERSION 3.2)
project(LearnOpenGL)

add_subdirectory("LearnOpenGL")

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)

add_subdirectory("vendor/glad")
add_subdirectory("vendor/glfw")
add_subdirectory("vendor/spdlog")
add_subdirectory("vendor/fmt")