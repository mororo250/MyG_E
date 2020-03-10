#pragma once

#include "Core/Texture/BasicTexture2D.h"
#include "Foundation/Uncopyable.h"

class Filter : public Uncopyable
{
public:

	Filter() = default;
	virtual ~Filter() = default;

	virtual void imgui_renderer() {}
	virtual void apply_filter(BasicTexture2D& input_texture) = 0;
};
