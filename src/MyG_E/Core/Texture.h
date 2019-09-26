#pragma once

#include "Foundation/Math/Vector.h"

class Texture
{
public:
	Texture(std::string const& file_path);
	Texture(Vector<float, 3> const& color = {1.0f, 1.0f, 1.0f}); // Create a 1 x 1 texture of a specTific color.
	Texture(Texture const& other);
	Texture& operator=(Texture const& other);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;
	
	inline int get_width() const { return m_width; } 
	inline int get_height() const { return m_height; }
	inline std::string get_filepath() const { return m_file_path; }
	inline float* get_color() const { return m_color.get(); }

private:
	void copy_other(Texture const& other);
	void create_texture(unsigned char const* local_buffer); 

	std::unique_ptr<float> m_color;
	unsigned int m_texture;
	std::string m_file_path;
	
	int m_width;
	int m_height;
	int m_bpp; // ???number of elements in a pixel ex: rgb = 3 rgba 4
};
