#pragma once

#include "Light.h"

class SpotLight : public Light
{
public:
	SpotLight(Vector<float, 3> const& position, const Vector<float, 3> const& color = Vector<float, 3>({ 1.0f, 1.0f, 1.0f }),
		float in_angle = 0.436332f, float out_angle = 0.523599f,
		Vector<float, 3>const& direction = Vector<float, 3>({ 0.0f, -1.0f, 0.0f }));
	~SpotLight() { s_count--; }

	inline float GetInAngle() { return m_in_angle; }
	inline void SetInAngle(float angle) { m_in_angle = angle; }

	inline float GetOutAngle() { return m_out_angle; }
	inline void SetOutAngle(float angle) { m_out_angle = angle; }

	inline Vector<float, 3> GetDirection() { return m_direction; }
	inline void SetDirection(Vector<float, 3> direction) { m_direction = direction; }

	void ImGuiRenderer() override;
	void set_uniform(Shader const* shader) override;

	static unsigned short get_count() { return s_count; }

private:
	float m_in_angle;
	float m_out_angle;
	Vector<float, 3> m_direction;
	static unsigned short s_count;
	static unsigned short s_id;
};
