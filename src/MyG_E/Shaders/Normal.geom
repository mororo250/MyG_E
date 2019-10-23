#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 3) out;

in vec3 v_normal[];
in vec4 v_position[];

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

const float MAGNITUDE = 0.1;

void generate_line(int index)
{
    gl_Position = v_position[index] * u_Model * u_View * u_Projection;
    EmitVertex();
    gl_Position = (v_position[index] * u_Model + vec4(v_normal[index], 0.0) * MAGNITUDE ) * u_View * u_Projection;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    generate_line(0); // first vertex normal
    generate_line(1); // second vertex normal
    generate_line(2); // third vertex normal
}
