#pragma once

#include "Core\Renderer.h"

class Renderer3D : public Renderer
{
public:
	Renderer3D();
	~Renderer3D();

	void draw_element(const IndexBuffer& ib) const override;
	void clear() const override;
};