#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 tex_coord;

out vec4 v_position;
out vec3 v_normal;
out vec2 v_tex_coord;
out mat3 v_world_normals;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform vec2 u_scale_uv;

void main()
{
	// Position
	v_position = vec4(position, 1.0) * u_Model;
	gl_Position = v_position * u_View * u_Projection;
	
	// Normals
	mat3 normal_matrix = mat3(transpose(inverse(u_Model)));
	v_normal = normalize(normal * normal_matrix);
	vec3 bt = tangent;
	vec3 t = bitangent;
	v_world_normals = transpose(normal_matrix) * mat3(t, bt, normal);

	// Textcoord
	v_tex_coord = tex_coord;
	v_tex_coord[0] *= u_scale_uv[0];
	v_tex_coord[1] *= u_scale_uv[1];
};
