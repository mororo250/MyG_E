#version 450 core

#define MAX_NUM_POINT_LIGHT 10
#define MAX_NUM_SPOT_LIGHT 10
#define MAX_NUM_DIRECTIONAL_LIGHT 5
#define MAX_NUM_SHADOW_CASTER_DIRECTIONAL_LIGTH 5
#define MAX_NUM_CASCADE_LAYERS 5

layout(location = 0) out vec4 frag_color;


struct Material 
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct AmbientLight
{
	vec3 color;
	float strength;	
};

struct Light
{
	vec3 color;
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

in vec4 v_position;
in vec3 v_normal;
in vec2 v_tex_coord;
in mat3 v_world_normals;
in vec4 v_pos_light_space[MAX_NUM_SHADOW_CASTER_DIRECTIONAL_LIGTH];
in float v_dist_to_light[MAX_NUM_SHADOW_CASTER_DIRECTIONAL_LIGTH];



// Material
uniform sampler2D u_texture;
uniform sampler2D u_specular_map;
uniform sampler2D u_nomal_map;
uniform usampler2D u_shadow_map[MAX_NUM_SHADOW_CASTER_DIRECTIONAL_LIGTH][MAX_NUM_CASCADE_LAYERS];
uniform bool u_is_using_normal_map;
uniform float u_shininess;
Material material;

// Others
uniform vec3 u_view_pos;
uniform int u_shadow_softness;

// Lights:
uniform uint u_NUM_POINT_LIGHT;
uniform uint u_NUM_SPOT_LIGHT;
uniform uint u_NUM_DIRECTIONAL_LIGHT;
uniform uint u_NUM_SHADOW_CASTER_DIRECTIONAL_LIGTH; 

uniform AmbientLight u_ambient_light;
uniform PointLight u_point_light[MAX_NUM_POINT_LIGHT];
uniform SpotLight u_spot_light[MAX_NUM_SPOT_LIGHT];
uniform DirectionalLight u_directional_light[MAX_NUM_DIRECTIONAL_LIGHT];
uniform DirectionalLight u_shadow_caster_directional_light[MAX_NUM_SHADOW_CASTER_DIRECTIONAL_LIGTH];

double texel_size = 0.0146484375;

vec3 light(Light light, vec3 ray_direction)
{
	vec3 norm = normalize(v_normal);
	if (u_is_using_normal_map)
	{
		norm = texture(u_nomal_map, v_tex_coord).rgb;
		// transform normal vector to range [-1,1]
		norm = normalize(norm * 2.0f - 1.0f);
		norm = normalize(v_world_normals * norm);
	}
	vec3 view_direction = normalize(u_view_pos - v_position.xyz);
	vec3 reflect_direction = reflect(ray_direction, norm);

// Difuse
	float diff = max(dot(norm, -ray_direction), 0.0f);
	vec3 diffuse = diff * light.diffuse_strength * light.color * material.diffuse;

// Specular
	vec3 halfway_dir = normalize(-ray_direction + view_direction);  
	float spec = pow(max(dot(norm, halfway_dir), 0.0f), material.shininess * 32);
	vec3 specular = light.specular_strength * spec * light.color * material.specular;

	return  diffuse + specular;
}

vec3 point_light(const uint i)
{
	vec3 light_direction = normalize(u_point_light[i].position - v_position.xyz);

// Attenuation
	float distance = length(u_point_light[i].position - v_position.xyz);
	float attenuation = 1.0f / (u_point_light[i].constant + u_point_light[i].linear * distance + u_point_light[i].quadratic * distance * distance);
	
	vec3 light = light(u_point_light[i].light, -light_direction);
	return light * attenuation;
}

vec3 spot_light(const uint i)
{
	vec3 light_direction = normalize(u_spot_light[i].position - v_position.xyz);
	
	float theta = dot(u_spot_light[i].direction, -light_direction);
	float epsilon = (u_spot_light[i].in_angle - u_spot_light[i].out_angle);
	float intensity = clamp((theta - u_spot_light[i].out_angle)/ epsilon , 0.0f, 1.0f);
	
	vec3 light = light(u_spot_light[i].light, -light_direction);
	return light * intensity;
}

vec3 directional_light(const uint i)
{
	vec3 light = light(u_directional_light[i].light, u_directional_light[i].directional);
	return light;
}

float reduce_light_bleeding(float v, float min_amount)
{
  return clamp((v - min_amount) / (1.0f - min_amount), 0.0f, 1.0f);
}

// get value from a single pixel -> Debug propose. 
dvec2 single_texel_value(uint shadow_id, vec2 coords, vec2 offset)
{
	uvec4 p1 = texture(u_shadow_map[shadow_id][0], coords);
	uvec4 p2 = texture(u_shadow_map[shadow_id][0], vec2(coords.x - offset.x, coords.y - offset.y));
	uvec4 p3 = texture(u_shadow_map[shadow_id][0], vec2(coords.x, coords.y - offset.y));
	uvec4 p4 = texture(u_shadow_map[shadow_id][0], vec2(coords.x - offset.x, coords.y));

	return dvec2(packDouble2x32(p1.xy) + packDouble2x32(p2.xy) 
	- packDouble2x32(p3.xy) - packDouble2x32(p4.xy), packDouble2x32(p1.zw) + packDouble2x32(p2.zw) 
	- packDouble2x32(p3.zw) - packDouble2x32(p4.zw));
}

dvec2 sample_sat_sum(uint shadow_id, vec2 coords, vec2 tile_size)
{
	uvec4 p1 = texture(u_shadow_map[shadow_id][0], coords); // (0, 0)
	uvec4 p2 = texture(u_shadow_map[shadow_id][0], coords + tile_size); // (1, 1)
	uvec4 p3 = texture(u_shadow_map[shadow_id][0], coords + vec2(tile_size.x, 0.0f)); // (1, 0)
	uvec4 p4 = texture(u_shadow_map[shadow_id][0],coords + vec2(0.0f, tile_size.y)); // (0, 1)
	return dvec2(packDouble2x32(p1.xy) + packDouble2x32(p2.xy) - packDouble2x32(p3.xy) - packDouble2x32(p4.xy),
		packDouble2x32(p1.zw) + packDouble2x32(p2.zw) - packDouble2x32(p3.zw) - packDouble2x32(p4.zw)); 
}

void sat_bilinear_filter(inout vec2 coords, vec2 texture_size, out vec4 weights)
{
	vec2 texel_size = 1.0f / texture_size;
	vec2 texel_coords = texture_size * coords;
	  
	// Compute Weight
	weights.xy = fract(texel_coords + 0.5f);
	weights.zw = 1.0f - weights.xy;
	weights = weights.xzxz * weights.wyyw;

	// Compute upper-left pixel coordinates
	coords = (floor(texel_coords - 0.5f) + 0.5f)* texel_size; // move uv to texel centre.
}

// Compute filter size and return upper left corner of the tile.
void sat_filter_tile(inout vec2 coords, vec2 texture_size, out vec2 tile_size)
{
	vec2 texel_size = 1.0f / texture_size;

	// Compute the filter size.
	tile_size = round(u_shadow_softness / 1024.0f *  texture_size); // Ensures that blur area will be the same for any shadow resolution.
	tile_size *= texel_size;

	// Compute upper left size.
	coords = coords - 0.5 * (tile_size - texel_size);
}

float shadow_calculation(const uint i)
{
	// shadow map coords
	vec2 light_coords = v_pos_light_space[i].xy / v_pos_light_space[i].w;
	light_coords = light_coords * 0.5f + 0.5f; // Set coordinates to the range {0, 1}

	// int n_pixel_offset = 1;
	vec2 texture_size = textureSize(u_shadow_map[i][0], 0);
	vec2 texel_size = 1 / texture_size;

	vec2 tile_size;
	vec4 weight;
	sat_filter_tile(light_coords, texture_size, tile_size);
	sat_bilinear_filter(light_coords, texture_size, weight);
	
	// summed area table. 
	dvec2 m1 = sample_sat_sum(i, light_coords + vec2(texel_size.x, 0), tile_size); //(1, 0)
	dvec2 m2 = sample_sat_sum(i, light_coords + vec2(0, texel_size.y), tile_size ); //(0, 1)
	dvec2 m3 = sample_sat_sum(i, light_coords + texel_size, tile_size); //(1, 1)
	dvec2 m4 = sample_sat_sum(i, light_coords, tile_size); //(0, 0)
	
	// Calculate the area. 
	tile_size = tile_size * texture_size; // Compute size in pixels
	double inv_area = 1 / (tile_size.x * tile_size.y);
	dvec2 m = (m1 * weight.x + m2 * weight.y + m3 * weight.z + m4 * weight.w) * inv_area;

	// dvec2 m = single_texel_value(i, light_coords, texel_size);

	// One-tailed inequality valid if v_dist_to_light[i] > Moments.x
	if (v_dist_to_light[i] <= m.x)
		return 1.0;
	
	// Compute variance.
	double variance = m.y - m.x*m.x;
	variance = max(variance, 0.002);

	// Compute probabilistic upper bound.
	double diff = v_dist_to_light[i]- m.x;
	float p_max = float(variance / (variance + diff*diff));

	// Linearly rescale [0.0f, min_amount] to (min_amount, 1.0f].
	float min_amount = 0.2f;
	return reduce_light_bleeding(p_max, min_amount); // decrease light bleeding
}

vec3 shadow_caster_directional_light(const uint i)
{
	vec3 light = light(u_shadow_caster_directional_light[i].light, u_shadow_caster_directional_light[i].directional);
	return light * shadow_calculation(i);
}

void main()
{
	material.diffuse = texture(u_texture, v_tex_coord).xyz;
	material.specular = texture(u_specular_map, v_tex_coord).xyz;
	material.shininess = u_shininess;

	vec3 result = vec3(0.0f);
	for(uint i = 0u; i < u_NUM_POINT_LIGHT; i++)
		result += point_light(i);
	for(uint i = 0u; i < u_NUM_DIRECTIONAL_LIGHT; i++)
		result += directional_light(i);
	for(uint i = 0u; i < u_NUM_SHADOW_CASTER_DIRECTIONAL_LIGTH; i++)
		result += shadow_caster_directional_light(i);
	for(uint i = 0u; i < u_NUM_SPOT_LIGHT; i++)
		result += spot_light(i);
	result += u_ambient_light.strength * u_ambient_light.color * material.diffuse; 

	frag_color = vec4(result, 1.0f);
};
