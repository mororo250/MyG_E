#pragma once

#include "Core/Texture/Texture2D.h"

class Material
{
public:
	Material(Texture2D* diff = new Texture2D(), Texture2D* spec = new Texture2D(), 
		Texture2D* norm_map  = new Texture2D(), float shi = 1.0f);
	Material(Material const& other);
	Material& operator=(Material const& other);
	~Material() = default;

	inline Texture2D const* get_diffuse() const { return m_diffuse.get(); }
	inline void set_diffuse(std::string const& file_path) { m_diffuse->change_texture(file_path); }
	inline void set_diffuse(Vector3f const& color) { m_diffuse->change_texture(color); }
	inline void set_diffuse(Texture2D const& texture) { m_diffuse.reset(new Texture2D(texture)); }

	inline Texture2D const* get_specular() const { return m_specular.get(); }
	inline void set_specular(std::string const& file_path) { m_specular->change_texture(file_path); }
	inline void set_specular(Vector3f const& color) { m_specular->change_texture(color); }
	inline void set_specular(Texture2D const& texture) { m_specular.reset(new Texture2D(texture)); }

	inline Texture2D const* get_normal_map() const { return m_normal_map.get(); }
	inline void set_normal_map(Texture2D const& texture) { m_normal_map.reset(new Texture2D(texture)); }
	inline bool is_using_normal_map() const { return m_use_normal_map; }

	inline float get_shininess() const { return m_shininess; }
	inline void set_shininess(float shininess) { m_shininess = shininess; }

	void imgui_renderer();
private:
	void copy_other(Material const& other);

	std::unique_ptr<Texture2D> m_diffuse; // a color or a texture
	std::unique_ptr<Texture2D> m_specular; // a color or a specular map
	std::unique_ptr<Texture2D> m_normal_map; // normal map
	float m_shininess;
	bool m_use_normal_map;
};
