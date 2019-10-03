#pragma once

#include "Foundation/Math/Vector.h"

class Texture
{
public:
	Texture() = default;
	~Texture() = default;

	virtual void bind(unsigned int slot = 0) const = 0;
	virtual void unbind() const = 0;

	virtual void imgui_renderer(std::string const& texture_name) {}
};
