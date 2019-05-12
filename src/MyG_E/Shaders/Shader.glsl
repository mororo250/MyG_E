//call once for each vertex
#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
out vec2 v_texCoord;

uniform mat3 u_MVP;

void main()
{
	vec3 aux = vec3(position, 1.0) * u_MVP;
	gl_Position = vec4(aux, 1.0);
	v_texCoord = texCoord;
};

//rasterization - call once for each pixel
#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;

uniform sampler2D u_texture;

void main()
{
	color = texture(u_texture, v_texCoord);
};

