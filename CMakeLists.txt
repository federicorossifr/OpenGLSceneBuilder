cmake_minimum_required(VERSION 3.10)
project(opengl_test)

set(CMAKE_CXX_STANDARD 17)

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE proj_srcs "src/*.cpp" "src/*.cc")
message(STATUS ${proj_srcs})
add_library(gl_proj SHARED ${proj_srcs})
target_link_libraries(gl_proj PUBLIC glu32 glfw3)

add_executable(main examples/main.cpp examples/test_shader_handler.cpp)
target_link_libraries(main PUBLIC gl_proj)

add_executable(oth examples/other.cpp)
target_link_libraries(oth PUBLIC gl_proj)

add_executable(test_handle examples/test_shader_handler.cpp)
target_link_libraries(test_handle PUBLIC gl_proj)
message(STATUS ${shaders})

add_custom_target(force_shaders)

file(GLOB_RECURSE shaders "shaders/*.cc")
add_custom_command(
        TARGET test_handle force_shaders POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy  ${shaders} "${CMAKE_CURRENT_BINARY_DIR}/"
)
