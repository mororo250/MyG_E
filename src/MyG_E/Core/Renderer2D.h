#pragma once

#include "Core\Renderer.h"

class Renderer2D : public Renderer
{
public:
	Renderer2D() = default;
	~Renderer2D() = default;

	void draw_element(const IndexBuffer& ib) const override;
	void clear() const override;
};
