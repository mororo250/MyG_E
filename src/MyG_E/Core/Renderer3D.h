#pragma once

#include "Foundation/Math/Vector.h"

class Renderer3D
{
public:

	Renderer3D();
	~Renderer3D();

	static void draw_element(class IndexBuffer const& ib);
	static void draw_quad();

	static void clear(Vector4f const& color = { 0.0f, 0.0f, 0.0f, 1.0f });
	static void clear_depth_buffer();
	static void clear_color_buffer(Vector4f const& color = { 0.0f, 0.0f, 0.0f, 1.0f });

	static void init();
	static void end();

	inline static bool has_init() { return s_has_init; }

private:
	static bool s_has_init;
};
