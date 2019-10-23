#pragma once
#include "Foundation/Uncopyable.h"

class Layer : public Uncopyable
{
public:
	Layer() = default;
	virtual ~Layer() = default;

	virtual void update() {}
	virtual void event() {}
	virtual void imgui_renderer() {}
};
