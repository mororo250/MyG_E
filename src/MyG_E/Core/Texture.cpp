#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Foundation/Gldebug.h"
#include "Texture.h"

#include "stb_image.h"


Texture::Texture(const std::string& file_path)
:m_texture(0) , m_file_path(file_path), m_local_buffer(nullptr),
m_bpp(0), m_width(0), m_height(0)
{
	stbi_set_flip_vertically_on_load(1); //flip the image - opengl start from the top left intead of the bottom lefet
	m_local_buffer = stbi_load(m_file_path.c_str(), &m_width, &m_height, &m_bpp, 4);
	
	GLcall(glGenTextures(1 , &m_texture));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_texture));

	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLcall(glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer));
	GLcall(glBindTexture(GL_TEXTURE_2D, 0)); //unbind

	if (m_local_buffer)
		stbi_image_free(m_local_buffer);
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
