#version 330 core

layout(location = 0) out vec4 frag_color;
in vec2 v_tex_coord;

uniform sampler2D u_texture;
uniform float u_offset;

void main()
{
	frag_color = texture(u_texture, v_tex_coord.xy) + texture(u_texture, vec2(v_tex_coord.x - u_offset / textureSize(u_texture, 0).x, v_tex_coord.y));
}
