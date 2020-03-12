#version 450 core

out vec4 frag_color;

in vec2 v_tex_coord;

uniform usampler2D u_shadow_map;

void main ()
{
	// summed area table 15 x 15 
	vec2 offset = 1.0f/ textureSize(u_shadow_map, 0);
	uvec2 p1 = texture(u_shadow_map, v_tex_coord).xy;
	uvec2 p2 = texture(u_shadow_map, vec2(v_tex_coord.x - offset.x, v_tex_coord.y - offset.y)).xy;
	uvec2 p3 = texture(u_shadow_map, vec2(v_tex_coord.x, v_tex_coord.y - offset.y)).xy;
	uvec2 p4 = texture(u_shadow_map, vec2(v_tex_coord.x - offset.x, v_tex_coord.y)).xy;

	double m = packDouble2x32(p1.xy) + packDouble2x32(p2.xy) 
	- packDouble2x32(p3.xy) - packDouble2x32(p4.xy);

	frag_color = vec4(vec3(m / 50.0f), 1.0f); 
}