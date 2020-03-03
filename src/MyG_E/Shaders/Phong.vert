#version 330 core

#define MAX_NUM_SHADOW_CASTER_DIRECTIONAL_LIGTH 5

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 tex_coord;

out vec4 v_position;
out vec3 v_normal;
out vec2 v_tex_coord;
out mat3 v_world_normals;
out vec4 v_pos_light_space[MAX_NUM_SHADOW_CASTER_DIRECTIONAL_LIGTH];
out float v_dist_to_light[MAX_NUM_SHADOW_CASTER_DIRECTIONAL_LIGTH];

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

// Light
uniform uint u_NUM_SHADOW_CASTER_DIRECTIONAL_LIGTH;
uniform mat4 u_light_view[MAX_NUM_SHADOW_CASTER_DIRECTIONAL_LIGTH];
uniform mat4 u_light_persp[MAX_NUM_SHADOW_CASTER_DIRECTIONAL_LIGTH];

uniform vec2 u_scale_uv;

void main()
{
	// Position
	v_position = vec4(position, 1.0f) * u_model;
	gl_Position = v_position * u_view * u_projection;
	
	// Normals
	mat3 normal_matrix = mat3(transpose(inverse(u_model)));
	v_normal = normalize(normal * normal_matrix);
	vec3 bt = tangent;
	vec3 t = bitangent;
	v_world_normals = transpose(normal_matrix) * mat3(t, bt, normal);

	// Textcoord
	v_tex_coord = tex_coord;
	v_tex_coord[0] *= u_scale_uv[0];
	v_tex_coord[1] *= u_scale_uv[1];

	// Light Space
	for(uint i = 0u; i < u_NUM_SHADOW_CASTER_DIRECTIONAL_LIGTH; i++)
	{
		v_pos_light_space[i] = v_position * u_light_view[i];
		v_dist_to_light[i] = v_pos_light_space[i].z * -1.0f;
		v_pos_light_space[i] *= u_light_persp[i];
	}
};
