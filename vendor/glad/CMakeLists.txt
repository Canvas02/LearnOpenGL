cmake_minimum_required(VERSION 3.2)
project(glad LANGUAGES C)

set(GLAD_INC_DIR ${PROJECT_SOURCE_DIR}/include)
set(GLAD_INC 
	include/glad/glad.h
	include/KHR/khrplatform.h
)

add_library(${PROJECT_NAME} src/glad.c ${GLAD_INC})
target_include_directories(${PROJECT_NAME} PUBLIC ${GLAD_INC_DIR})