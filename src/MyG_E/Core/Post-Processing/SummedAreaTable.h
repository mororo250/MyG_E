#pragma once

#include "Core/FrameBuffer.h"
#include "Core/Texture/BasicTexture2D.h"

#include "Foundation/Math/Vector.h"

class SummeadAreaTable
{
public:
	SummeadAreaTable(BasicTexture2D const* tex, Vector2i const& shadow_resolution);
	~SummeadAreaTable() = default;

	void gen_sat(Vector2i const& shadow_size);

private:
	BasicTexture2D m_sat_texture;
	BasicTexture2D const* m_input_texture;
	FrameBuffer m_fbo;
};
