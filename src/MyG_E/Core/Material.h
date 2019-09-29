#pragma once

#include "Core/Texture.h"

class Material
{
public:
	Material(Texture* diff = new Texture(), Texture* spec = new Texture(), float shi = 32.0f);
	Material(Material const& other);
	Material& operator=(Material const& other);
	~Material() = default;

	inline Texture* get_diffuse() const { return m_diffuse.get(); }
	inline void set_diffuse(std::string const& file_path) { m_diffuse->change_texture(file_path); }
	inline void set_diffuse(Vector<float, 3> const& color) { m_diffuse->change_texture(color); }
	inline void set_diffuse(Texture const& texture) { m_diffuse.reset(new Texture(texture)); }

	inline Texture* get_specular() const { return m_specular.get(); }
	inline void set_specular(std::string const& file_path) { m_specular->change_texture(file_path); }
	inline void set_specular(Vector<float, 3> const& color) { m_specular->change_texture(color); }
	inline void set_specular(Texture const& texture) { m_specular.reset(new Texture(texture)); }

	inline float get_shininess() const { return m_shininess; }
	inline void set_shininess(float shininess) { m_shininess = shininess; }


private:
	void copy_other(Material const& other);

	std::unique_ptr<Texture> m_diffuse; // a color or a texture
	std::unique_ptr<Texture> m_specular; // a color or a specular map
	float m_shininess;
};