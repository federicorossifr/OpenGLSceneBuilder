cmake_minimum_required(VERSION 3.10)
project(opengl_test)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Ofast -march=native -std=c++2a)
include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE proj_srcs "src/*.cpp" "src/*.cc")
message(STATUS ${proj_srcs})
add_library(gl_proj SHARED ${proj_srcs})
if(WIN32)
	target_link_libraries(gl_proj PUBLIC glu32 glfw3)
else()
	target_link_libraries(gl_proj PUBLIC glfw GLU)
endif()


add_executable(sceneTest examples/jsonSceneTest.cpp)
target_link_libraries(sceneTest PUBLIC gl_proj)

add_executable(debug_static examples/jsonSceneTest.cpp ${proj_srcs})
target_link_libraries(debug_static PUBLIC glu32 glfw3 -static)
target_compile_options(debug_static PUBLIC -static-libgcc -static-libstdc++ )
target_link_libraries(debug_static PUBLIC  -static-libgcc -static-libstdc++ -static)

file(GLOB_RECURSE shaders "shaders/*.vert" "shaders/*.frag" "shaders/*.geom")
foreach(GLSL  ${shaders})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
            COMMAND ${CMAKE_COMMAND} -E copy ${GLSL} ${SPIRV}
            DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

# TESTd
