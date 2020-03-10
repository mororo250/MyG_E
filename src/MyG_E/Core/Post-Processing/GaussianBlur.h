#pragma once

#include "Core/Post-Processing/Filter.h"
#include "Core/Shader.h"

class GaussianBlur : public Filter
{
public:
	GaussianBlur(int const kernel_size = 5, float const sigma = 1.0f);
	~GaussianBlur() = default;

	void imgui_renderer();
	void apply_filter(BasicTexture2D& input_texture);

private:
	int m_kernel_size;
	float m_sigma;
};
