cmake_minimum_required(VERSION 3.10)
project(opengl_test)

set(CMAKE_CXX_STANDARD 17)

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE proj_srcs "src/*.cpp" "src/*.cc")
message(STATUS ${proj_srcs})
add_library(gl_proj SHARED ${proj_srcs})

if(WIN32)
	target_link_libraries(gl_proj PUBLIC glu32 glfw3)
else()
	target_link_libraries(gl_proj PUBLIC glfw GLU)
endif()

add_executable(main examples/main.cpp examples/test_shader_handler.cpp)
target_link_libraries(main PUBLIC gl_proj)

add_executable(oth examples/other.cpp)
target_link_libraries(oth PUBLIC gl_proj)

add_executable(test_handle examples/test_shader_handler.cpp)
target_link_libraries(test_handle PUBLIC gl_proj)

add_executable(test_glm examples/test_glm.cpp)
target_link_libraries(test_glm PUBLIC gl_proj)

add_executable(test_encaps_glapp examples/test_encaps_glapp.cpp)
target_link_libraries(test_encaps_glapp PUBLIC gl_proj)

add_executable(random_cubes examples/randomCubes.cpp)
target_link_libraries(random_cubes PUBLIC gl_proj)

add_executable(lighting examples/lighting.cpp)
target_link_libraries(lighting PUBLIC gl_proj)

add_executable(lighting2 examples/lighting2.cpp)
target_link_libraries(lighting2 PUBLIC gl_proj)

add_executable(debug_static examples/lighting2.cpp ${proj_srcs})
target_link_libraries(debug_static PUBLIC glu32 glfw3 -static)
target_compile_options(debug_static PUBLIC -static-libgcc -static-libstdc++ )
target_link_libraries(debug_static PUBLIC  -static-libgcc -static-libstdc++ -static)

file(GLOB_RECURSE shaders "shaders/*.vert" "shaders/*.frag" )
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
