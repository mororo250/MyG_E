#version 450 core

// This can not be use for any texture. It was made for variance shadow maps.

layout(location = 0) out uvec4 frag_color;
in vec2 v_tex_coord;

uniform usampler2D u_texture;
uniform float u_offset;

void main()
{
	float offset = u_offset / textureSize(u_texture, 0).y;
	
	uvec4 texel1 = texture(u_texture, v_tex_coord);
	uvec4 texel2 = texture(u_texture, vec2(v_tex_coord.x, v_tex_coord.y - offset));
	uvec4 texel3 = texture(u_texture, vec2(v_tex_coord.x, v_tex_coord.y - 2 * offset));
	uvec4 texel4 = texture(u_texture, vec2(v_tex_coord.x, v_tex_coord.y - 3 * offset));
	
	dvec2 dcolor = dvec2(packDouble2x32(texel1.xy), packDouble2x32(texel1.zw)) 
	+ dvec2(packDouble2x32(texel2.xy), packDouble2x32(texel2.zw))
	+ dvec2(packDouble2x32(texel3.xy), packDouble2x32(texel3.zw))
	+ dvec2(packDouble2x32(texel4.xy), packDouble2x32(texel4.zw));

	frag_color = uvec4(unpackDouble2x32(dcolor.x), unpackDouble2x32(dcolor.y));
}
