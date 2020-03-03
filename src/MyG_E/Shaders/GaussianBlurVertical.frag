#version 330 core

uniform float sigma;     // The sigma value for the gaussian function: higher value means more blur
                         // A good value for 9x9 is around 3 to 5
                         // A good value for 7x7 is around 2.5 to 4
                         // A good value for 5x5 is around 2 to 3.5
                         // ... play around with this based on what you need :)

uniform float blurSize;  // This should usually be equal to
                         // 1.0f / texture_pixel_width for a horizontal blur, and
                         // 1.0f / texture_pixel_height for a vertical blur.

uniform sampler2D blurSampler;  // Texture that will be blurred by this shader

const float pi = 3.14159265f;