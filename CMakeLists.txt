cmake_minimum_required(VERSION 3.2)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory("LearnOpenGL")

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)

# set(SPDLOG_FMT_EXTERNAL ON)
# set(fmt_DIR "${CMAKE_BINARY_DIR}/vendor/fmt")

add_subdirectory("vendor/glad")
add_subdirectory("vendor/glfw")
add_subdirectory("vendor/spdlog")
add_subdirectory("vendor/fmt")
