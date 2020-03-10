#pragma once

#include "Core/RenderBuffer.h"

class BasicTexture2D;
class Texture2DMultisample;

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

	FrameBuffer();
	~FrameBuffer();

	void bind() const;
	void unbind() const;

	// You need to bind the framebuffer before attach or detach anything.
	void attach_texture(BasicTexture2D const& tex, Attachment attachment) const;
	void attach_texture(Texture2DMultisample const& tex, Attachment attachment) const;
	void attach_rbo(Attachment attachment, RenderBuffer& rbo) const;
	void detach_texture(Attachment attachment) const;
	void detach_rbo(Attachment attachment) const;

	void disable_color_buffer() const;

	inline unsigned int get_fbo_id() const { return m_fbo; }
	
	// Debug
	void check_status() const;

private:
	unsigned int m_fbo;
};
