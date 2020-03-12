#version 450 core

layout(location = 0) out vec4 Frag_color;

uniform vec3 u_Color;

void main()
{
	Frag_color = vec4(u_Color, 1.0);
};
