#pragma once

#include "Core/Renderer.h"

class Renderer3D
{
public:

	Renderer3D();
	~Renderer3D();

	static void draw_element(class IndexBuffer const& ib);
	static void draw_quad();

	static void clear();
	static void clear_buffer();

	static void init();
	static void end();

	inline static bool has_init() { return s_has_init; }

private:
	static bool s_has_init;
};
