#pragma once

class FrameBuffer
{
public:
	enum Attachment
	{
		COLOR_ATTACHMENT = 0x8CE0, // GL_COLOR_ATTACHMENT0,
		DEPTH_ATTACHMENT = 0x8D00, // GL_DEPTH_ATTACHMENT,
		STENCIL_ATTACHMENT = 0x8D20, // GL_STENCIL_ATTACHMENT,
		DEPTH_STENCIL_ATTACHMENT = 0x821A // GL_DEPTH_STENCIL_ATTACHMENT
	};

	FrameBuffer(unsigned int tex_id, Attachment attach);
	~FrameBuffer();

	void bind() const;
	void unbind() const;

	void disable_color_buffer() const;

	inline unsigned int get_fbo() const { return m_fbo; }

private:
	unsigned int m_fbo;
};
