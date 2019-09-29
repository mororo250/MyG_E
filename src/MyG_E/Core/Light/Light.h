#pragma once

#include "Foundation/Math/Vector.h"
#include "Core\Model3D.h"

class Light
{
public:
	Light(Vector<float, 3> const& position, Vector<float, 3> const& color);
	Light(Light const& other);
	Light& operator=(Light const& other);
	virtual ~Light();

	inline Vector<float, 3> get_light_position() const { return m_model->get_position(); }
	inline void set_light_position(Vector<float, 3> const& position) { m_model->set_position(position); }

	inline Vector<float, 3> get_light_color() const { return m_color; }
	inline void set_light_color(Vector<float, 3> const& color) { m_color = color; }

	inline float get_ambient_strength() const { return m_ambient_strength; }
	inline void set_ambient_strength(const float ambient_strength) { m_ambient_strength = ambient_strength; }

	inline float get_diffuse_strength() const { return m_diffuse_strength; }
	inline void set_diffuse_strength(const float diffuse_strength) { m_diffuse_strength = diffuse_strength; }
	
	inline float get_specular_strength() const { return m_specular_strength; }
	inline void set_specular_strength(const float specular_strength) { m_specular_strength = specular_strength; }

	Model3D* GetModel() { return m_model; }

	virtual void ImGuiRenderer();
	virtual void set_uniform(class Shader const* shader) = 0;

protected:
	void set_general_uniform(Shader const* shader, std::string const& uniform_name);

private:
	void copy_other(Light const& other);

	Vector<float, 3> m_color;
	Model3D* m_model;
	float m_ambient_strength;
	float m_diffuse_strength;
	float m_specular_strength;
};