#pragma once

#include "Core/Texture/BasicTexture2D.h"
#include "Foundation/Uncopyable.h"

class Filter : public Uncopyable
{
public:

	Filter() = default;
	virtual ~Filter() = default;

	virtual void imgui_renderer() = 0;
	virtual void apply_filter(BasicTexture2D const& input_texture) = 0;
};
