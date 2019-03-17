//call once for each vertex
#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 Color;
out vec4 V_Color;

uniform mat3 u_MVP;

void main()
{
	vec3 aux = vec3(position, 1.0) * u_MVP;
	gl_Position = vec4(aux, 1.0);
	V_Color = Color;
};

//rasterization - call once for each pixel
#shader fragment
#version 330 core

layout(location = 0) out vec4 Frag_color;

in vec4 v_Color;

void main()
{
	Frag_color = v_Color;
};