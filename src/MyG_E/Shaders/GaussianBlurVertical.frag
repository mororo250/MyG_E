#version 330 core

in vec2 v_tex_coord;

uniform sampler2D u_image;

uniform float u_sigma;	// The sigma value for the gaussian function: higher value means more blur
						// A good value for 9x9 is around 3 to 5
						// A good value for 7x7 is around 2.5 to 4
						// A good value for 5x5 is around 2 to 3.5
						// ... play around with this based on what you need :)

uniform float u_blur_size;	// This should usually be equal to
							// 1.0f / texture_pixel_width for a horizontal blur, and
							// 1.0f / texture_pixel_height for a vertical blur.

uniform sampler2D u_blur_Sampler;  // Texture that will be blurred by this shader

const float PI = 3.14159265f;

void get_gaussian_kernel()
{
	vec3 g; 
	g.x = 1.0 / (sqrt(2.0 * PI) * u_sigma); 
	g.y = exp(-0.5 * u_blur_size * u_blur_size / (u_sigma * u_sigma)); 
	g.z = g.y * g.y; 
	for (i = 0; i < N; i++) 
	{
		// MultiplySomethingByTheGaussianCoefficient(g.x);
		g.xy *= g.yz; 
	}
}

void main()
{
    
}