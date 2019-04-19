//call once for each vertex
#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 Color;

out vec4 v_Color;
out vec4 v_Position;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
	v_Position = vec4(position, 1.0) * u_Model;
	gl_Position = v_Position * u_ViewProjection;
	v_Color = Color;
};

//rasterization - call once for each pixel
#shader fragment
#version 330 core

layout(location = 0) out vec4 Frag_color;

in vec4 v_Color;
in vec4 v_Position;

uniform vec3 u_LightColor;
//_uniform vec3 u_LightPos;

void main()
{
	Frag_color = v_Color * vec4(u_LightColor, 1.0);
};