#pragma once

#include "Foundation/Math/Vector.h"
#include "Core/ShadowMap.h"
#include "Core/Model3D.h"

class Light
{
public:
	Light(Vector3f const& position, Vector3f const& color);
	Light(Light const& other);
	Light& operator=(Light const& other);
	virtual ~Light();

	inline Vector3f get_light_position() const { return m_model->get_position(); }
	inline void set_light_position(Vector3f const& position) { m_model->set_position(position); }

	inline Vector3f get_light_color() const { return m_color; }
	inline void set_light_color(Vector3f const& color) { m_color = color; }

	inline float get_diffuse_strength() const { return m_diffuse_strength; }
	inline void set_diffuse_strength(float const diffuse_strength) { m_diffuse_strength = diffuse_strength; }
	
	inline float get_specular_strength() const { return m_specular_strength; }
	inline void set_specular_strength(float const specular_strength) { m_specular_strength = specular_strength; }

	Model3D* get_model() { return m_model; }

	virtual void imgui_renderer();
	virtual void set_uniform(class Shader const* shader) = 0;
	virtual inline ShadowMap const* get_shadow_map() { return nullptr; }

	virtual Matrix4x4f get_light_view() { return IDENTITY_4x4F; }
	virtual Matrix4x4f get_light_persp() { return IDENTITY_4x4F; }

protected:
	void set_general_uniform(Shader const* shader, std::string const& uniform_name);

private:
	void copy_other(Light const& other);

	Vector3f m_color;
	Model3D* m_model;
	float m_diffuse_strength;
	float m_specular_strength;
};