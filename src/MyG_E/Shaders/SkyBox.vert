#version 330 core

layout (location = 0) in vec3 position;

out vec3 v_tex_coord;;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 scale;

void main()
{
    v_tex_coord = position;
	vec4 pos = (vec4(position, 1.0) * scale * u_View * u_Projection);
	gl_Position = pos.xyww;
}
