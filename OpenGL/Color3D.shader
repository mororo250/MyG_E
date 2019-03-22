//call once for each vertex
#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 Color;
out vec4 V_Color;

uniform mat4 u_MVP;

void main()
{
	gl_Position = vec4(position, 1.0) * u_MVP;
	V_Color = Color;
};

//rasterization - call once for each pixel
#shader fragment
#version 330 core

layout(location = 0) out vec4 Frag_color;

in vec4 V_Color;

void main()
{
	Frag_color = V_Color;
};