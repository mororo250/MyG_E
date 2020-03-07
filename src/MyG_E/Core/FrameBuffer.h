#pragma once

#include "Core/RenderBuffer.h"

class FrameBuffer : public Uncopyable
{
public:
	enum Attachment
	{
		COLOR_ATTACHMENT = 0x8CE0, // GL_COLOR_ATTACHMENT0,
		DEPTH_ATTACHMENT = 0x8D00, // GL_DEPTH_ATTACHMENT,
		STENCIL_ATTACHMENT = 0x8D20, // GL_STENCIL_ATTACHMENT,
		DEPTH_STENCIL_ATTACHMENT = 0x821A // GL_DEPTH_STENCIL_ATTACHMENT
	};

	FrameBuffer(unsigned int tex_id, Attachment attachment);
	~FrameBuffer();

	void bind() const;
	void unbind() const;

	void change_texture(unsigned int tex_id, Attachment attachment);
	void attach_rbo(Attachment attachment, RenderBuffer& rbo) const;
	void detach_rbo(Attachment attachment) const;

	void disable_color_buffer() const;

	inline unsigned int get_fbo_id() const { return m_fbo; }
	
private:
	unsigned int m_fbo;
};
