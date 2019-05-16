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

struct PointLight 
{
	// Gneral.
	vec3 position;
	vec3 color;
	float ambient_strength;
	float diffuse_strength;
	float specular_strength;

	// Attenuation.
	float constant;
	float linear;
	float quadratic;
};

struct DirectionalLight 
{
	// Gneral.
	vec3 position;
	vec3 color;
	float ambient_strength;
	float diffuse_strength;
	float specular_strength;

	// Directional
	vec3 directional;
};

#define MAX_POINT_LIGHT 10
#define MAX_DIRECTIONAL_LIGHT 10

in vec4 v_Position;
in vec3 v_Normal;

uniform Material u_Material;
uniform int u_NumPointLight;
uniform int u_NumDirectionalLight;
uniform vec3 u_ViewPos;

uniform PointLight u_PointLight[MAX_POINT_LIGHT];
uniform DirectionalLight u_DirectionalLight[MAX_DIRECTIONAL_LIGHT];

vec3 point_light(int i)
{
	vec3 norm = normalize(v_Normal);
	vec3 light_direction = normalize(u_PointLight[i].position - v_Position.xyz);
	vec3 view_direction = normalize(u_ViewPos - v_Position.xyz);
	vec3 reflect_direction = reflect(-light_direction, norm);

// Attenuation
	float distance = length(u_PointLight[i].position - v_Position.xyz);
	float attenuation = 1.0f / (u_PointLight[i].constant + u_PointLight[i].linear * distance + u_PointLight[i].quadratic * distance * distance);
	
// Ambient
	vec3 ambient = u_PointLight[i].ambient_strength * u_PointLight[i].color * u_Material.ambient;

// Difuse
	float diff = max(dot(norm, light_direction), 0.0);
	vec3 diffuse = diff * u_PointLight[i].diffuse_strength * u_PointLight[i].color * u_Material.diffuse;

// Specular
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), u_Material.shininess * 128);
	vec3 specular = u_PointLight[i].specular_strength * spec * u_PointLight[i].color * u_Material.specular;

	return (ambient + diffuse + specular) * attenuation;
}

vec3 directional_light(int i)
{
	vec3 norm = normalize(v_Normal);
	vec3 view_direction = normalize(u_ViewPos - v_Position.xyz);
	vec3 reflect_direction = reflect(u_DirectionalLight[i].directional, norm);

// Ambient
	vec3 ambient = u_DirectionalLight[i].ambient_strength * u_DirectionalLight[i].color * u_Material.ambient;

// Difuse
	float diff = max(dot(norm, -u_DirectionalLight[i].directional), 0.0);
	vec3 diffuse = diff * u_DirectionalLight[i].diffuse_strength * u_DirectionalLight[i].color * u_Material.diffuse;

// Specular
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), u_Material.shininess * 128);
	vec3 specular = u_DirectionalLight[i].specular_strength * spec * u_DirectionalLight[i].color * u_Material.specular;

	return (ambient + diffuse + specular);
}

void main()
{
	vec3 result = vec3(0.0);
	for(int i = 0; i < u_NumPointLight; i++)
		result += point_light(i);
	for(int i = 0; i < u_NumDirectionalLight; i++)
		result += directional_light(i);
	Frag_color = vec4(result, 1.0);
};