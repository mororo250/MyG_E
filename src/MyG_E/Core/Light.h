#pragma once

#include "Foundation/Math/Vector.h"
#include "Core\Model3D.h"

class Light
{
public:
	Light(Vector<float, 3> position, Vector<float, 3> color = { 1.0f, 1.0f, 1.0f });
	~Light();

	inline Vector<float, 3> GetLightPosition() const { return mModel.GetPosition(); }
	inline void SetLightPosition(const Vector<float, 3>& position) { mModel.SetPosition(position); }

	inline Vector<float, 3> GetLightColor() const { return mColor; }
	inline void SetLightColor(const Vector<float, 3>& color) { mColor = color; }

	inline float GetAmbientStength() const { return m_ambient_strength; }
	inline void SetAmbientStength(const float ambient_strength) { m_ambient_strength = ambient_strength; }

	inline float GetDiffuseStrength() const { return m_diffuse_strength; }
	inline void SetDiffuseStrength(const float diffuse_strength) { m_diffuse_strength = diffuse_strength; }
	
	inline float GetSpecularStrength() const { return m_specular_strength; }
	inline void SetSpecularStrength(const float specular_strength) { m_specular_strength = specular_strength; }

	inline Model3D GetModel() const { return mModel; }

	void ImGuiRenderer();

private:
	Vector<float, 3> mColor;
	std::shared_ptr<Mesh> mMesh;
	Model3D mModel;
	float m_ambient_strength;
	float m_diffuse_strength;
	float m_specular_strength;
};