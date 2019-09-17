#pragma once

#include "Foundation/Math/Vector.h"
#include "Core\Model3D.h"

class Light
{
public:
	Light(const Vector<float, 3>& position, const Vector<float, 3>& color);
	virtual ~Light() = default;

	inline Vector<float, 3> GetLightPosition() const { return m_model.GetPosition(); }
	inline void SetLightPosition(const Vector<float, 3>& position) { m_model.SetPosition(position); }

	inline Vector<float, 3> GetLightColor() const { return m_color; }
	inline void SetLightColor(const Vector<float, 3>& color) { m_color = color; }

	inline float GetAmbientStength() const { return m_ambient_strength; }
	inline void SetAmbientStength(const float ambient_strength) { m_ambient_strength = ambient_strength; }

	inline float GetDiffuseStrength() const { return m_diffuse_strength; }
	inline void SetDiffuseStrength(const float diffuse_strength) { m_diffuse_strength = diffuse_strength; }
	
	inline float GetSpecularStrength() const { return m_specular_strength; }
	inline void SetSpecularStrength(const float specular_strength) { m_specular_strength = specular_strength; }

	inline Model3D& GetModel() { return m_model; }

	virtual void ImGuiRenderer();
	virtual void SetUniform(class Shader* shader) = 0;

private:
	Vector<float, 3> m_color;
	Model3D m_model;
	float m_ambient_strength;
	float m_diffuse_strength;
	float m_specular_strength;
};