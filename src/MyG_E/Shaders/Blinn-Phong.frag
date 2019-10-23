#version 330 core

layout(location = 0) out vec4 Frag_color;


struct Material 
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 color;
	float ambient_strength;
	float diffuse_strength;
	float specular_strength;
};

struct PointLight 
{
	vec3 position;
	Light light;

	// Attenuation.
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	Light light;

	float in_angle;
	float out_angle;
	vec3 direction;
};

struct DirectionalLight 
{
	Light light;
	vec3 directional;
};

#define MAX_LIGHT 10

in vec4 v_position;
in vec3 v_normal;
in vec2 v_tex_coord;
in mat3 v_world_normals;

// Material
uniform sampler2D u_texture;
uniform sampler2D u_specular_map;
uniform sampler2D u_nomal_map;
uniform bool u_is_using_normal_map;
uniform float u_shininess;
Material material;

uniform vec3 u_ViewPos;

// Lights:
uniform int u_NumPointLight;
uniform int u_NumSpotLight;
uniform int u_NumDirectionalLight;
uniform PointLight u_PointLight[MAX_LIGHT];
uniform SpotLight u_SpotLight[MAX_LIGHT];
uniform DirectionalLight u_DirectionalLight[MAX_LIGHT];

vec3 light(Light light, vec3 ray_direction)
{
	vec3 norm = normalize(v_normal);
	if (u_is_using_normal_map)
	{
		norm = texture(u_nomal_map, v_tex_coord).rgb;
		// transform normal vector to range [-1,1]
		norm = normalize(norm * 2.0 - 1.0);
		norm = normalize(v_world_normals * norm);
	}
	vec3 view_direction = normalize(u_ViewPos - v_position.xyz);
	vec3 reflect_direction = reflect(ray_direction, norm);

// Ambient
	vec3 ambient = light.ambient_strength * light.color * material.diffuse;

// Difuse
	float diff = max(dot(norm, -ray_direction), 0.0);
	vec3 diffuse = diff * light.diffuse_strength * light.color * material.diffuse;

// Specular
	vec3 halfway_dir = normalize(-ray_direction + view_direction);  
	float spec = pow(max(dot(norm, halfway_dir), 0.0), material.shininess * 32);
	vec3 specular = light.specular_strength * spec * light.color * material.specular;

	return ambient + diffuse + specular;
}

vec3 point_light(const int i)
{
	vec3 light_direction = normalize(u_PointLight[i].position - v_position.xyz);

// Attenuation
	float distance = length(u_PointLight[i].position - v_position.xyz);
	float attenuation = 1.0f / (u_PointLight[i].constant + u_PointLight[i].linear * distance + u_PointLight[i].quadratic * distance * distance);
	
	vec3 light = light(u_PointLight[i].light, -light_direction);
	return light * attenuation;
}

vec3 spot_light(const int i)
{
	vec3 light_direction = normalize(u_SpotLight[i].position - v_position.xyz);
	
	float theta = dot(u_SpotLight[i].direction, -light_direction);
	float epsilon = (u_SpotLight[i].in_angle - u_SpotLight[i].out_angle);
	float intensity = clamp((theta - u_SpotLight[i].out_angle)/ epsilon , 0.0, 1.0);
	
	vec3 light = light(u_SpotLight[i].light, -light_direction);
	return light * intensity;
}

vec3 directional_light(const int i)
{
	vec3 light = light(u_DirectionalLight[i].light, u_DirectionalLight[i].directional);
	return light;
}

void main()
{
	material.diffuse = texture(u_texture, v_tex_coord).xyz;
	material.specular = texture(u_specular_map, v_tex_coord).xyz;
	material.shininess = u_shininess;

	vec3 result = vec3(0.0);
	for(int i = 0; i < u_NumPointLight; i++)
		result += point_light(i);
	for(int i = 0; i < u_NumDirectionalLight; i++)
		result += directional_light(i);
	for(int i = 0; i < u_NumSpotLight; i++)
		result += spot_light(i);

	Frag_color = vec4(result, 1.0);
};
