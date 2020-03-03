#include "RenderBuffer.h"

#include <glad/glad.h>

#include "Foundation/Gldebug.h"

RenderBuffer::RenderBuffer(InternalFormat internal_format, unsigned int width, unsigned int height)
{
	GLcall(glGenRenderbuffers(1, &m_rbo));
	GLcall(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
	GLcall(glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height));
	GLcall(glBindRenderbuffer(GL_RENDERBUFFER, 0)); // Unbind
}

RenderBuffer::~RenderBuffer()
{
	GLcall(glDeleteRenderbuffers(1, &m_rbo));
}

void RenderBuffer::bind() const
{
	GLcall(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
}

void RenderBuffer::unbind() const
{
	GLcall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void RenderBuffer::respecify_rbo(InternalFormat internal_format, unsigned int width, unsigned int height)
{
	GLcall(glDeleteRenderbuffers(1, &m_rbo));
	GLcall(glGenRenderbuffers(1, &m_rbo));
	GLcall(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
	GLcall(glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height));
	GLcall(glBindRenderbuffer(GL_RENDERBUFFER, 0)); // Unbind
}
