#pragma once

#include "Core\Light\Light.h"

class PointLight : public Light
{
public:
	PointLight(const Vector3f& position, const Vector3f& color = { 1.0f, 1.0f, 1.0f });
	~PointLight() { s_count--; }
	
	inline Vector3f GetAttenuationConstants() const { return m_attenuation_constants; }
	inline float GetConstant() const { return m_attenuation_constants[0]; }
	inline float GetLinear() const { return m_attenuation_constants[1]; }
	inline float GetQuadratic() const { return m_attenuation_constants[2]; }
	inline void SetAttenuationConstants(const Vector3f& constants) { m_attenuation_constants = constants; }

	void imgui_renderer() override;
	void set_uniform(Shader const* shader) override;

	static unsigned short get_count() { return s_count; }

private:

	// Attenuation (constant, linear, quadratic).
	Vector3f m_attenuation_constants;
	static unsigned short s_count;
};
