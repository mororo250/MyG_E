#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 v_normal;
out vec4 v_position;

uniform mat4 u_model;

void main()
{
	v_position =  vec4(position, 1.0);
	mat3 normal_matrix = mat3(transpose(inverse(u_model)));
	v_normal = normalize(normal * normal_matrix);
};
