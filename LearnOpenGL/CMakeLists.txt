
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
find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE
	glad::glad 
	spdlog::spdlog 
	fmt::fmt
	glfw 
    stb
    glm
)

set(RESOURCE_FILES
    "res/shaders/basic.frag.glsl"
    "res/shaders/basic.vert.glsl"

    "res/textures/awesomeface.png"
    "res/textures/wood_container.jpg"
)

foreach(f IN LISTS RESOURCE_FILES)
    configure_file(${f} ${f} COPYONLY)
endforeach()

# add_custom_command(
#     TARGET ${PROJECT_NAME} PRE_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#         "${CMAKE_SOURCE_DIR}/LearnOpenGL/res"
#         "${CMAKE_BINARY_DIR}/LearnOpenGL/res"
# )

if(WIN32)
target_compile_definitions(${PROJECT_NAME} PRIVATE "WINDOWS")
elseif(UNIX)
target_compile_definitions(${PROJECT_NAME} PRIVATE "POSIX")
endif()
