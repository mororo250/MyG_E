#version 450 core

// Vertex shader shared by many frag shaders as Gaussian filters, ShadowMapDebug and DrawTexture.

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coord;

out vec2 v_tex_coord;

void main()
{
	v_tex_coord = tex_coord;
	gl_Position = vec4(position, 1.0);
}
