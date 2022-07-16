cmake_minimum_required(VERSION 3.2)
project(stb LANGUAGES C)

set(STB_FILES
    "include/stb_image.h"
)

add_library(${PROJECT_NAME} INTERFACE ${STB_FILES})
target_include_directories(${PROJECT_NAME} INTERFACE "include")