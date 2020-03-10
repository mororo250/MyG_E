#include "Texture2DMultisample.h"

#include <glad/glad.h>
#include "Foundation/Gldebug.h"

Texture2DMultisample::Texture2DMultisample(unsigned int const num_samples, InternalFormat const internal_format, 
	int const width, int const height, void* data)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, num_samples, internal_format, width, height, GL_TRUE);
	GLcall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
}

Texture2DMultisample::~Texture2DMultisample()
{
	GLcall(glDeleteTextures(1, &m_texture));
}

void Texture2DMultisample::respecify_textute(unsigned int const num_samples, InternalFormat const internal_format, int const width, int const height, void* data) const
{
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, num_samples, internal_format, width, height, false);
	GLcall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
}

void Texture2DMultisample::bind(unsigned int slot) const
{
	GLcall(glActiveTexture(GL_TEXTURE0 + slot));
	GLcall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture));
}

void Texture2DMultisample::unbind() const
{
	GLcall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
}
