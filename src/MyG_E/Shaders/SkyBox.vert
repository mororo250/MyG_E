#version 330 core

layout (location = 0) in vec3 position;

out vec3 v_tex_coord;;

uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec3 u_camera_position;

void main()
{
    v_tex_coord = position;
	vec4 pos = (vec4(u_camera_position + position, 1.0) * u_view * u_projection);
	gl_Position = pos.xyww;
}
