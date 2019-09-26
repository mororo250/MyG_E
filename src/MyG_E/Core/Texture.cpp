#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Foundation/Gldebug.h"
#include "Texture.h"

#include "stb_image.h"


Texture::Texture(std::string const& file_path)
	:m_texture(0),
	m_color(nullptr),
	m_file_path(file_path), 
	m_bpp(0), 
	m_width(0), 
	m_height(0)
{
	stbi_set_flip_vertically_on_load(1); // Flip the image - opengl start from the top left intead of the bottom lefet
	unsigned char* local_buffer = stbi_load(m_file_path.c_str(), &m_width, &m_height, &m_bpp, 4);
	
	create_texture(local_buffer);

	if (local_buffer)
		stbi_image_free(local_buffer);
}

Texture::Texture(Vector<float, 3> const& color)
	:m_texture(0),
	m_color(new float[3]{ color[0], color[1], color[2] }),
	m_file_path(""),
	m_bpp(4),
	m_width(1),
	m_height(1)
{
	unsigned char* local_buffer = new unsigned char[4]{ static_cast<unsigned char>(color[0] * 255.0f),
		static_cast<unsigned char>(color[1] * 255.0f), static_cast<unsigned char>(color[2] * 255.0f), 255 };
	m_width = 1;
	m_height = 1;
	
	create_texture(local_buffer);

	delete local_buffer;
}

Texture::Texture(Texture const& other)
{
	copy_other(other);
}

Texture& Texture::operator=(Texture const& other)
{
	copy_other(other);
	return *this;
}

Texture::~Texture()
{
	GLcall(glDeleteTextures(1, &m_texture));
}

void Texture::bind(unsigned int slot) const
{
	GLcall(glActiveTexture(GL_TEXTURE0 + slot));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_texture));
}

void Texture::unbind() const
{
	GLcall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::copy_other(Texture const& other)
{
	m_texture = m_texture;
	m_file_path = m_file_path;

	m_width = other.m_width;
	m_height = other.m_height;
	m_bpp = other.m_bpp;

	m_color.reset(nullptr);
	if (other.m_color)
		m_color.reset(new float[3]);
		memcpy(m_color.get(), other.m_color.get(), 3);
}

void Texture::create_texture(unsigned char const* local_buffer)
{
	GLcall(glGenTextures(1, &m_texture));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_texture));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLcall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer));
	GLcall(glBindTexture(GL_TEXTURE_2D, 0)); //unbind
}
