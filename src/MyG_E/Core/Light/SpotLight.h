#pragma once

#include "Light.h"

class SpotLight : public Light
{
public:
	SpotLight(Vector3f const& position, Vector3f const& color = Vector3f({ 1.0f, 1.0f, 1.0f }),
		float in_angle = 0.436332f, float out_angle = 0.523599f,
		Vector3f const& direction = Vector3f({ 0.0f, -1.0f, 0.0f }));
	~SpotLight() { s_count--; }

	inline float GetInAngle() { return m_in_angle; }
	inline void SetInAngle(float angle) { m_in_angle = angle; }

	inline float GetOutAngle() { return m_out_angle; }
	inline void SetOutAngle(float angle) { m_out_angle = angle; }

	inline Vector3f GetDirection() { return m_direction; }
	inline void SetDirection(Vector3f direction) { m_direction = direction; }

	void ImGuiRenderer() override;
	void set_uniform(Shader const* shader) override;

	static unsigned short get_count() { return s_count; }

private:
	float m_in_angle;
	float m_out_angle;
	Vector3f m_direction;
	static unsigned short s_count;
	static unsigned short s_id;
};
