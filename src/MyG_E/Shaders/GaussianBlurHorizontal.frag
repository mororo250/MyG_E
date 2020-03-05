#version 330 core

layout(location = 0) out vec4 frag_color;
in vec2 v_tex_coord;

uniform sampler2D u_texture;  // Texture that will be blurred by this shader
uniform uint u_kernel_size;
uniform float u_sigma;	// The sigma value for the gaussian function: higher value means more blur
						// A good value for 9x9 is around 3 to 5
						// A good value for 7x7 is around 2.5 to 4
						// A good value for 5x5 is around 2 to 3.5
						// ... play around with this based on what you need :)

const float PI = 3.14159265f;

void main()
{
	vec2 proj_coords = v_tex_coord;
	float offset = 1.0f / textureSize(u_texture, 0).x; 
	float sigma = u_sigma / textureSize(u_texture, 0).x;
	vec3 g; 
	g.x = 1.0f / (sqrt(2.0f * PI) * u_sigma); 
	g.y = exp(-0.5f * offset * offset / (sigma * sigma)); 
	g.z = g.y * g.y; 

	vec3 color = texture(u_texture, proj_coords.xy).xyz * g.x;
	float weight_normalizer = g.x; // As the total weight can be more or less than 1.0 this normalize the weights.
	for (uint i = 0u; i < u_kernel_size; i++) 
	{
		g.xy *= g.yz; 
		weight_normalizer += 2 * g.x;
		color += texture(u_texture, vec2(proj_coords.x + offset, proj_coords.y)).xyz * g.x;
		color += texture(u_texture, vec2(proj_coords.x - offset, proj_coords.y)).xyz * g.x;
		offset += offset;
	}
	frag_color = vec4(color / weight_normalizer, 1.0f);
}
