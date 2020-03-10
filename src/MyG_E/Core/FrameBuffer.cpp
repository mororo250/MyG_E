#include "FrameBuffer.h"

#include <glad/glad.h>

#include "Foundation/Gldebug.h"
#include "Core/Texture/BasicTexture2D.h"
#include "Core/Texture/Texture2DMultisample.h"

FrameBuffer::FrameBuffer()
{
	GLcall(glGenFramebuffers(1, &m_fbo));
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

void FrameBuffer::attach_texture(BasicTexture2D const& tex, Attachment attachment) const
{
	GLcall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex.get_texture(), 0));
}

void FrameBuffer::attach_texture(Texture2DMultisample const& tex, Attachment attachment) const
{
	GLcall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D_MULTISAMPLE, tex.get_texture(), 0));
}

void FrameBuffer::detach_texture(Attachment attachment) const
{
	GLcall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, 0, 0));
}

void FrameBuffer::attach_rbo(Attachment attachment, RenderBuffer& rbo) const
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

void FrameBuffer::check_status() const
{
	unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "FB error, status: " << status << std::endl;;
		ASSERT(false);
	}
}