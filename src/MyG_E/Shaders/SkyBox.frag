#version 330 core

out vec4 frag_Color;

in vec3 v_tex_coord;

uniform samplerCube u_skybox;

void main()
{    
    frag_Color = texture(u_skybox, v_tex_coord);
}