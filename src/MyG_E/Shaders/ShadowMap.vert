#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 u_light_view;
uniform mat4 u_light_persp;
uniform mat4 u_model;

out float v_dist_to_light;

void main()
{
	vec4 aux = vec4(position, 1.0f) * u_model * u_light_view;
	v_dist_to_light = aux.z * -1.0; // Right hand notation
	gl_Position = aux * u_light_persp;
}
