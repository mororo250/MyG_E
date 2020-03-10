#version 330 core
out vec4 frag_color;

in vec2 v_tex_coord;

uniform sampler2D u_shadow_map;
uniform float u_aux;

void main ()
{
	vec2 texture_size = textureSize(u_shadow_map, 0);
	frag_color = vec4(vec3(texture(u_shadow_map, v_tex_coord).r 
	- texture(u_shadow_map, vec2(v_tex_coord.x, v_tex_coord.y - 1.0 / texture_size.y)).r
	- texture(u_shadow_map, vec2(v_tex_coord.x - 1.0 / texture_size.x, v_tex_coord.x)).r 
	+ texture(u_shadow_map, vec2(v_tex_coord.x - 1.0 / texture_size.x, v_tex_coord.y - 1.0 / texture_size.y)).r ), 1.0);
	// frag_color = vec4(vec3(texture(u_shadow_map, v_tex_coord).r / texture(u_shadow_map, vec2(0.99, 0.99)).r), 1.0f)
	;}