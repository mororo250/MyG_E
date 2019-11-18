#version 330 core

out vec4 frag_color;
in vec2 v_tex_coord;
uniform sampler2D u_shadow_map;
uniform float u_aux;

void main ()
{
	//float depth_value = texture(u_shadow_map, v_tex_coord).x;
	frag_color = vec4(texture(u_shadow_map, v_tex_coord).xyz, 1.0);
}