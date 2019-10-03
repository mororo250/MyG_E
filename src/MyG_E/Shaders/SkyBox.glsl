//call once for each vertex
#shader vertex

#version 330 core
layout (location = 0) in vec3 position;

out vec3 v_tex_coord;;

uniform mat4 u_view_projection;
uniform mat4 scale;

void main()
{
    v_tex_coord = position;
	vec4 pos = (vec4(position, 1.0) * scale * u_view_projection);
	gl_Position = pos.xyww;
}

// rasterization - call once for each pixel
#shader fragment

#version 330 core
out vec4 FragColor;

in vec3 v_tex_coord;

uniform samplerCube u_skybox;

void main()
{    
    FragColor = texture(u_skybox, v_tex_coord);
}