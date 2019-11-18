#include "FrameBuffer.h"

#include <glad/glad.h>

#include "Foundation/Gldebug.h"

FrameBuffer::FrameBuffer(unsigned int tex_id, Attachment attach)
{
	GLcall(glGenFramebuffers(1, &m_fbo));

	// Attach texture as FBO.
	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
	GLcall(glFramebufferTexture2D(GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, tex_id, 0));

	unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "FB error, status: 0x%x" << status << std::endl;;
		ASSERT(false);
	}
}

FrameBuffer::~FrameBuffer()
{
	GLcall(glDeleteBuffers(1, &m_fbo));
}

void FrameBuffer::bind() const
{
	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
}

void FrameBuffer::unbind() const
{
	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::disable_color_buffer() const
{
	GLcall(glDrawBuffer(GL_NONE));
	GLcall(glReadBuffer(GL_NONE));
}
