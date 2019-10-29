#pragma once

#include "Core\Renderer.h"

class Renderer3D : public Renderer
{
public:
	Renderer3D();
	~Renderer3D();

	void draw_element(const IndexBuffer& ib) const override;
	void custom_draw() const;
	void clear() const override;

	void set_active() override;
	void set_disactive() override;
};
