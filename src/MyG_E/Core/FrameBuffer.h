#pragma once

#include "Core/RenderBuffer.h"

class BasicTexture2D;
class Texture2DMultisample;

class FrameBuffer : public Uncopyable
{
public:
	enum Attachment
	{
		COLOR_ATTACHMENT0 = 0x8CE0, // GL_COLOR_ATTACHMENT0
		COLOR_ATTACHMENT1 = 0x8CE1, // GL_COLOR_ATTACHMENT1
		COLOR_ATTACHMENT2 = 0x8CE2, // GL_COLOR_ATTACHMENT2
		COLOR_ATTACHMENT3 = 0x8CE3, // GL_COLOR_ATTACHMENT3
		COLOR_ATTACHMENT4 = 0x8CE4, // GL_COLOR_ATTACHMENT4
		COLOR_ATTACHMENT5 = 0x8CE5, // GL_COLOR_ATTACHMENT5
		COLOR_ATTACHMENT6 = 0x8CE6, // GL_COLOR_ATTACHMENT6
		COLOR_ATTACHMENT7 = 0x8CE7, // GL_COLOR_ATTACHMENT7
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
	void attach_rbo(RenderBuffer& rbo, Attachment attachment) const;
	void detach_texture(Attachment attachment) const;
	void detach_rbo(Attachment attachment) const;

	void disable_color_buffer() const;

	inline unsigned int get_fbo_id() const { return m_fbo; }
	
	// Debug
	void check_status() const;

private:
	unsigned int m_fbo;
};
