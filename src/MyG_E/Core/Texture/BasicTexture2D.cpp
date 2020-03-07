#include "BasicTexture2D.h"

#include <glad/glad.h>
#include "Foundation/Gldebug.h"

BasicTexture2D::BasicTexture2D(BasicTexture2D::TextureFilter const mim_filter, BasicTexture2D::TextureFilter const mag_filter, 
	BasicTexture2D::TextureWrap const s, BasicTexture2D::TextureWrap const t, int const detail_level, 
	BasicTexture2D::InternalFormat const internal_format, int const width, int const height, 
	BasicTexture2D::Format const format, BasicTexture2D::DataType type, void* data)
{
	gen_texture(mim_filter, mag_filter, s, t, detail_level, internal_format, width, height, format, type, data);
}

BasicTexture2D::~BasicTexture2D()
{
	GLcall(glDeleteTextures(1, &m_texture));
}

void BasicTexture2D::respecify_textute( int const detail_level, BasicTexture2D::InternalFormat const internal_format, 
	int const width, int const height, BasicTexture2D::Format const format, BasicTexture2D::DataType type, void* data) const
{
	GLcall(glBindTexture(GL_TEXTURE_2D, m_texture)); // bind
	GLcall(glTexImage2D(GL_TEXTURE_2D, detail_level, internal_format, width, height, 0, format, type, data));
	GLcall(glBindTexture(GL_TEXTURE_2D, 0)); // unbind
}

void BasicTexture2D::gen_mipmaps() const
{
	glGenerateMipmap(GL_TEXTURE_2D);
}

void BasicTexture2D::bind(unsigned int slot) const
{
	GLcall(glActiveTexture(GL_TEXTURE0 + slot));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_texture));
}

void BasicTexture2D::unbind() const
{
	GLcall(glBindTexture(GL_TEXTURE_2D, 0));
}

void BasicTexture2D::gen_texture(TextureFilter const mim_filter, TextureFilter const mag_filter, TextureWrap const s, TextureWrap const t, int const detail_level, InternalFormat const internal_format, int const width, int const height, Format const format, DataType const type, void* data)
{
	GLcall(glGenTextures(1, &m_texture));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_texture)); // bind
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mim_filter));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t));

	GLcall(glTexImage2D(GL_TEXTURE_2D, detail_level, internal_format, width, height, 0, format, type, data));
	GLcall(glBindTexture(GL_TEXTURE_2D, 0)); // unbind
}
