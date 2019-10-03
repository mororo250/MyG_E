#pragma once

#include "Core/Texture/Texture.h"
#include "Foundation/Math/Vector.h"

class Texture2D : public Texture
{
public:
	Texture2D(std::string const& file_path);
	Texture2D(Vector<float, 3> const& color = { 1.0f, 1.0f, 1.0f }); // Create a 1 x 1 texture of a specTific color.
	Texture2D(Texture2D const& other);
	Texture2D& operator=(Texture2D const& other);
	~Texture2D();

	void bind(unsigned int slot = 0) const override;
	void unbind() const override;

	inline int get_width() const { return m_width; }
	inline int get_height() const { return m_height; }
	inline bool is_unitary() const { return m_is_unitary; }
	inline std::string get_filepath() const { return m_file_path; }
	inline float const* get_color() const { return m_color.get(); }

	void change_texture(std::string const& file_path);
	void change_texture(Vector<float, 3> const& color);

	void imgui_renderer(std::string const& texture_name) override;

private:
	void copy_other(Texture2D const& other);

	void if_is_a_color();
	void if_is_a_texture();

	void create_texture(unsigned char const* local_buffer);

	std::unique_ptr<float[] > m_color;
	unsigned int m_texture;
	std::string m_file_path;

	int m_width;
	int m_height;
	int m_nrChannels; // number of channels

	bool m_is_unitary; // Ture if texture is a 1 x 1.
};
