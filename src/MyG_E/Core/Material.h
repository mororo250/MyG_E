#pragma once

#include "Core/Texture/Texture2D.h"

class Material
{
public:
	Material(Texture2D* diff = new Texture2D(), Texture2D* spec = new Texture2D(), float shi = 32.0f);
	Material(Material const& other);
	Material& operator=(Material const& other);
	~Material() = default;

	inline Texture2D* get_diffuse() const { return m_diffuse.get(); }
	inline void set_diffuse(std::string const& file_path) { m_diffuse->change_texture(file_path); }
	inline void set_diffuse(Vector<float, 3> const& color) { m_diffuse->change_texture(color); }
	inline void set_diffuse(Texture2D const& texture) { m_diffuse.reset(new Texture2D(texture)); }

	inline Texture2D* get_specular() const { return m_specular.get(); }
	inline void set_specular(std::string const& file_path) { m_specular->change_texture(file_path); }
	inline void set_specular(Vector<float, 3> const& color) { m_specular->change_texture(color); }
	inline void set_specular(Texture2D const& texture) { m_specular.reset(new Texture2D(texture)); }

	inline float get_shininess() const { return m_shininess; }
	inline void set_shininess(float shininess) { m_shininess = shininess; }


private:
	void copy_other(Material const& other);

	std::unique_ptr<Texture2D> m_diffuse; // a color or a texture
	std::unique_ptr<Texture2D> m_specular; // a color or a specular map
	float m_shininess;
};