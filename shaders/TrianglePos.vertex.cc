#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0

out vec4 vertexColor; // specify a color output to the fragment shader
uniform mat4 rotation;
uniform vec4 translation; // Test 2
uniform vec4 color;
void main()
{
    vec4 pos = vec4(aPos,1.0);
    gl_Position = rotation*pos + translation; // see how we directly give a vec3 to vec4's constructor
    vertexColor = color; // set the output variable to a dark-red color
}