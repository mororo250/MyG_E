#pragma once

#include "Core\Light\Light.h"


class DirectionalLight : public Light
{
public:
	DirectionalLight(const Vector<float, 3>& position,
		const Vector<float, 3>& color = Vector<float, 3>({ 1.0f, 1.0f, 1.0f }),
		const Vector<float, 3>& directional = Vector<float, 3>({0.0f, -1.0f, 0.0f}));
	~DirectionalLight() { s_count--; }

	inline Vector<float, 3> GetDirectionalLight() { return m_direction; }
	inline void SetDirectionalLight(Vector<float, 3>& direction) { m_direction = direction; }

	void ImGuiRenderer() override;
	void SetUniform(Shader* shader) override;

	static unsigned short get_count() { return s_count; }

private:
	Vector<float, 3> m_direction;
	static unsigned short s_count;
	unsigned short m_id;
};
