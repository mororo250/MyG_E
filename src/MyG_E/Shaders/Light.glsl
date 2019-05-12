#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
	gl_Position = vec4(position, 1.0) * u_Model * u_ViewProjection;

};

//rasterization - call once for each pixel
#shader fragment
#version 330 core

layout(location = 0) out vec4 Frag_color;

uniform vec3 u_Color;

void main()
{
	Frag_color = vec4(u_Color, 1.0);
};