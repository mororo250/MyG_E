#version 450 core

layout(location = 0) out uvec4 variance_shadow_map;

in float v_dist_to_light;

// Where to split the value. 8 bits works well for most situations.
float g_DistributeFactor = 256; 
vec4 DistributePrecision(vec2 Moments) 
{  
	float FactorInv = 1 / g_DistributeFactor;	// Split precision
	vec2 IntPart;
	vec2 FracPart = modf(Moments * g_DistributeFactor, IntPart);	// Compose outputs to make reconstruction cheap.   
	return vec4(IntPart * FactorInv, FracPart); 
} 

vec2 RecombinePrecision(vec4 Value) 
{
	float FactorInv = 1 / g_DistributeFactor;  
	return (Value.zw * FactorInv + Value.xy); 
}

void main()
{
	double m1 = v_dist_to_light;
	// Compute partial derivatives of depth.
	double dx = dFdx(v_dist_to_light);
	double dy = dFdy(v_dist_to_light);
	// Compute second moment over the pixel extents.
	double m2 = v_dist_to_light * v_dist_to_light + 0.25 *(dx*dx + dy*dy);
	variance_shadow_map = uvec4(unpackDouble2x32(m1), unpackDouble2x32(m2));
}
