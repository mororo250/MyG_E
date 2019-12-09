#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 u_light_space;
uniform mat4 u_model;

void main()
{
    gl_Position = vec4(position, 1.0) * u_model * u_light_space;
}
