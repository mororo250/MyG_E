#version 330 core

out vec4 frag_color;

in vec2 v_tex_coord;

uniform sampler2D u_shadow_map;
uniform float u_aux;

void main ()
{
	frag_color = vec4(vec3(texture(u_shadow_map, v_tex_coord).r / 50.0f), 1.0);
}