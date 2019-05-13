//call once for each vertex
#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec4 v_Position;
out vec3 v_Normal;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
	v_Normal = normal;
	v_Position = vec4(position, 1.0) * u_Model;
	gl_Position = v_Position * u_ViewProjection;

};

//rasterization - call once for each pixel
#shader fragment
#version 330 core

layout(location = 0) out vec4 Frag_color;

struct Material 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light 
{
	vec3 position;
	vec3 color;
	float ambient_strength;
	float diffuse_strength;
	float specular_strength;
};

#define MAX_LIGHT 10

in vec4 v_Position;
in vec3 v_Normal;

uniform Material u_Material;
uniform Light u_Light[MAX_LIGHT];
uniform int u_NumLight;
uniform vec3 u_ViewPos;


vec3 point_light(int i)
{
	vec3 norm = normalize(v_Normal);
	vec3 light_direction = normalize(u_Light[i].position - v_Position.xyz);
	vec3 view_direction = normalize(u_ViewPos - v_Position.xyz);
	vec3 reflect_direction = reflect(-light_direction, norm);

// Ambient
	vec3 ambient = u_Light[i].ambient_strength * u_Light[i].color * u_Material.ambient;

	// Difuse
	float diff = max(dot(norm, light_direction), 0.0);
	vec3 diffuse = diff * u_Light[i].diffuse_strength * u_Light[i].color * u_Material.diffuse;

	// Specular
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), u_Material.shininess * 128);
	vec3 specular = u_Light[i].specular_strength * spec * u_Light[i].color * u_Material.specular;

	return ambient + diffuse + specular;
}

void main()
{
	vec3 result = vec3(0.0);
	for(int i = 0; i < u_NumLight; i++)
		result += point_light(i);
	Frag_color = vec4(result, 1.0);
};