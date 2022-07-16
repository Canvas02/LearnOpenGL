cmake_minimum_required(VERSION 3.2)
project(glad LANGUAGES C)

add_library(${PROJECT_NAME}
    "src/glad.c"
    "include/glad/glad.h"
    "include/KHR/khrplatform.h"
)
add_library(glad::glad ALIAS glad)

target_include_directories(${PROJECT_NAME} PUBLIC "include")
