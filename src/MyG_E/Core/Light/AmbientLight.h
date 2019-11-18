#pragma once

#include "Foundation/Math/Vector.h"

class AmbientLight
{
public:
	AmbientLight(float const strength = 0.1f, Vector3f const& color = {1.0f, 1.0f, 1.0f});
	~AmbientLight() = default;

	inline Vector3f get_light_color() const { return m_color; }
	inline void set_light_color(Vector3f const& color) { m_color = color; }

	inline float get_strength() const { return m_strength; }
	inline void set_strength(float const strength) { m_strength = strength; }

	void imgui_renderer();

private:
	float m_strength;
	Vector3f m_color;
};