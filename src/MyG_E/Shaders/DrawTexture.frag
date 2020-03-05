#version 330 core

out vec4 frag_color;

in vec2 v_tex_coord;

uniform sampler2D u_texture;

void main ()
{
	frag_color = vec4(texture(u_texture, v_tex_coord).xyz, 1.0f);
}