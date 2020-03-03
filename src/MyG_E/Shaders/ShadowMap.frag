#version 330 core

layout(location = 0) out vec2 variance_shadow_map;

in float v_dist_to_light;

void main()
{
	variance_shadow_map.x = v_dist_to_light;
	// Compute partial derivatives of depth.
	float dx = dFdx(v_dist_to_light);
	float dy = dFdy(v_dist_to_light);
	// Compute second moment over the pixel extents.
	variance_shadow_map.y = v_dist_to_light * v_dist_to_light + 0.25*(dx*dx + dy*dy);
}