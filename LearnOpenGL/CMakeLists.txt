
cmake_minimum_required(VERSION 3.2)
project(LearnOpenGL LANGUAGES CXX)

set(SRC_FILES
    "src/main.cxx"
    "src/callbacks.cxx"
    "src/file.cxx" 
    "src/shader_program.cxx"
)

set(HEADER_FILES
    "include/platfrom.h"
    "include/callbacks.h"
    "include/file.h"
    "include/shader_program.h"
    "include/gl_error.h"
 )

find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE
	glad::glad 
	spdlog::spdlog 
	fmt::fmt
	glfw 
)

file(COPY "res" DESTINATION "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")
# add_custom_command(
#     TARGET ${PROJECT_NAME} PRE_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#         "${CMAKE_SOURCE_DIR}/res"
#         "${CMAKE_BINARY_DIR}/res"
# )

if(WIN32)
target_compile_definitions(${PROJECT_NAME} PRIVATE "WINDOWS")
elseif(UNIX)
target_compile_definitions(${PROJECT_NAME} PRIVATE "POSIX")
endif()
