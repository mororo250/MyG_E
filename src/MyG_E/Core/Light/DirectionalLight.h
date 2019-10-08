#pragma once

#include "Core\Light\Light.h"


class DirectionalLight : public Light
{
public:
	DirectionalLight(Vector3f const& position,
		Vector3f const& color = Vector3f({ 1.0f, 1.0f, 1.0f }),
		Vector3f const& directional = Vector3f({0.0f, -1.0f, 0.0f}));
	~DirectionalLight() { s_count--; }

	inline Vector3f GetDirection() { return m_direction; }
	inline void SetDirection(Vector3f& direction) { m_direction = direction; }

	void ImGuiRenderer() override;
	void set_uniform(Shader const* shader) override;

	static unsigned short get_count() { return s_count; }

private:
	Vector3f m_direction;
	static unsigned short s_count;
	static unsigned short s_id;
};
