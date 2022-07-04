
cmake_minimum_required(VERSION 3.2)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

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
 )

 file(COPY "res" DESTINATION "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} glad glfw spdlog fmt::fmt)

if(WIN32)
target_compile_definitions(${PROJECT_NAME} PRIVATE "WINDOWS")
elseif(UNIX)
target_compile_definitions(${PROJECT_NAME} PRIVATE "POSIX")
endif()
