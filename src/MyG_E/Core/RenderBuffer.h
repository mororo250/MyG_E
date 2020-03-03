#pragma once

#include "Foundation\Uncopyable.h"

class RenderBuffer : public Uncopyable
{
public:
	enum InternalFormat
	{
		DEPTH_COMPONENT = 0x1902,
		DEPTH_STENCIL = 0x84F9,
		STENCIL = 0x1802,
		DEPTH_COMPONENT16 = 0x81A5,
		DEPTH_COMPONENT24 = 0x81A6,
		DEPTH_COMPONENT32F = 0x81A7,
		DEPTH24_STENCIL8 = 0x88F0,
		DEPTH32F_STENCIL8 = 0x8CAD,
		STENCIL_INDEX8 = 0x8D48
	};

	RenderBuffer(InternalFormat internal_format, unsigned int width, unsigned int height);
	~RenderBuffer();

	void bind() const;
	void unbind() const;

	void respecify_rbo(InternalFormat internal_format, unsigned int width, unsigned int height); // delete rbo and recreate it.
	inline unsigned int get_rbo_id() const { return m_rbo; }

private:
	unsigned int m_rbo;
};
