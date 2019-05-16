#pragma once

#include "Core\Light\Light.h"


class DirectionalLight : public Light
{
public:
	DirectionalLight(const Vector<float, 3>& position, const Vector<float, 3>& color = Vector<float, 3>({ 1.0f, 1.0f, 1.0f }), const Vector<float, 3>& directional = Vector<float, 3>({0.0f, -1.0f, 0.0f}));
	~DirectionalLight() = default;

	inline Vector<float, 3> GetDirectionalLight() { return m_directional; }
	inline void SetDirectionalLight(Vector<float, 3>& directional) { m_directional = directional; }

	void ImGuiRenderer() override;

private:
	Vector<float, 3> m_directional;
};