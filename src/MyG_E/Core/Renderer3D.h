#pragma once

#include "Core\Renderer.h"

class Renderer3D : public Renderer
{
public:
	Renderer3D();
	~Renderer3D();

	void Draw(const IndexBuffer& ib) const override;
	void Clear() const override;
};