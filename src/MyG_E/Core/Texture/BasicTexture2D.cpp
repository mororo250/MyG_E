#include "BasicTexture2D.h"

#include <glad/glad.h>
#include "Foundation/Gldebug.h"

BasicTexture2D::BasicTexture2D(TextureParm const& parm)
{
	gen_texture(parm);
}

BasicTexture2D::~BasicTexture2D()
{
	GLcall(glDeleteTextures(1, &m_texture));
}

void BasicTexture2D::respecify_textute(TextureParm const& parm) const
{
	GLcall(glBindTexture(GL_TEXTURE_2D, m_texture)); // bind
	GLcall(glTexImage2D(GL_TEXTURE_2D, parm.m_detail_level, parm.m_internal_format, parm.m_resolution[0], parm.m_resolution[1], 0, parm.m_format, parm.m_type, parm.m_data));
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

void BasicTexture2D::gen_texture(TextureParm const& parm)
{
	GLcall(glGenTextures(1, &m_texture));
	GLcall(glBindTexture(GL_TEXTURE_2D, m_texture)); // bind
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parm.m_mim_filter));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parm.m_mag_filter));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, parm.m_s));
	GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, parm.m_t));

	GLcall(glTexImage2D(GL_TEXTURE_2D, parm.m_detail_level, parm.m_internal_format, parm.m_resolution[0], parm.m_resolution[1], 0, parm.m_format, parm.m_type, parm.m_data));
	GLcall(glBindTexture(GL_TEXTURE_2D, 0)); // unbind
}

constexpr BasicTexture2D::TextureParm::TextureParm(TextureFilter const mim_filter, TextureFilter const mag_filter, TextureWrap const s,
	TextureWrap const t, int const detail_level, InternalFormat const internal_format, Vector2i const& res, Format const format, DataType const type, void* data)
	: m_mim_filter(mim_filter)
	, m_mag_filter(mag_filter)
	, m_s(s)
	, m_t(t)
	, m_detail_level(detail_level)
	, m_internal_format(internal_format)
	, m_resolution(res)
	, m_format(format)
	, m_type(type)
	, m_data(data)
{
}
