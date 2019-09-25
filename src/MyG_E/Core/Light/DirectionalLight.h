#pragma once

#include "Core\Light\Light.h"


class DirectionalLight : public Light
{
public:
	DirectionalLight(Vector<float, 3> const& position,
		Vector<float, 3> const& color = Vector<float, 3>({ 1.0f, 1.0f, 1.0f }),
		Vector<float, 3> const& directional = Vector<float, 3>({0.0f, -1.0f, 0.0f}));
	~DirectionalLight() { s_count--; }

	inline Vector<float, 3> GetDirection() { return m_direction; }
	inline void SetDirection(Vector<float, 3>& direction) { m_direction = direction; }

	void ImGuiRenderer() override;
	void SetUniform(Shader* shader) override;

	static unsigned short get_count() { return s_count; }

private:
	Vector<float, 3> m_direction;
	static unsigned short s_count;
	unsigned short m_id;
};
