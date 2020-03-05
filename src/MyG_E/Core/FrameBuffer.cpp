#include "FrameBuffer.h"

#include <glad/glad.h>

#include "Foundation/Gldebug.h"

FrameBuffer::FrameBuffer(unsigned int tex_id, Attachment attachment)
{
	GLcall(glGenFramebuffers(1, &m_fbo));

	// Attach texture as FBO.
	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo)); 
	GLcall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex_id, 0));

	unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "FB error, status: 0x%x" << status << std::endl;;
		ASSERT(false);
	}
	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

FrameBuffer::~FrameBuffer()
{
	GLcall(glDeleteFramebuffers(1, &m_fbo));
}

void FrameBuffer::bind() const
{
	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
}

void FrameBuffer::unbind() const
{
	GLcall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::attach_rbo(Attachment attachment, RenderBuffer& rbo) const // You need to bind the framebuffer before calling it
{
	GLcall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo.get_rbo_id()));
}

void FrameBuffer::detach_rbo(Attachment attachment) const
{
	GLcall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, 0));
}

void FrameBuffer::disable_color_buffer() const // You need to bind the framebuffer before calling it
{
	GLcall(glDrawBuffer(GL_NONE));
	GLcall(glReadBuffer(GL_NONE));
}
