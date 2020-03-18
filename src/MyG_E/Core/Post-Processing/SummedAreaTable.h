#pragma once

#include "Core/FrameBuffer.h"
#include "Core/Texture/BasicTexture2D.h"

#include "Foundation/Math/Vector.h"

class SummeadAreaTable
{
public:
	SummeadAreaTable(BasicTexture2D* sat_texture, Vector2i const& texture_resolution);
	~SummeadAreaTable() = default;

	void gen_sat(BasicTexture2D const& input_texture) const;

private:
	std::unique_ptr<BasicTexture2D> m_sat_texture;
	FrameBuffer m_fbo;
	Vector2i m_texture_resolution;
};
