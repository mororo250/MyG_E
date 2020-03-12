#pragma once

#include "Foundation\Uncopyable.h"

class RenderBuffer : public Uncopyable
{
public:
	enum InternalFormat
	{
		RED = 0x1903,				// GL_RED
		RGB = 0x1907,				// GL_RGB
		RGBA = 0x1908,				// GL_RGBA
		DEPTH_COMPONENT = 0x1902,
		DEPTH_STENCIL = 0x84F9,
		STENCIL = 0x1802,

		// Sized
		R8 = 0x8229,				// GL_R8
		R16 = 0x822A,				// GL_R16
		RG8 = 0x822B,				// GL_RG8
		RG16 = 0x822C,				// GL_RG16
		R16F = 0x822D,				// GL_R16F
		R32F = 0x822E,				// GL_R32F
		RG16F = 0x822F,				// GL_RG16F 
		RG32F = 0x8230,				// GL_RG32F 
		R8I = 0x8231,				// GL_R8I
		R8UI = 0x8232,				// GL_R8UI
		R16I = 0x8233,				// GL_R16I
		R16UI = 0x8234,				// GL_R16UI 
		R32I = 0x8235,				// GL_R32I
		R32UI = 0x8236,				// GL_R32UI 
		RG8I = 0x8237,				// GL_RG8I
		RG8UI = 0x8238,				// GL_RG8UI 
		RG16I = 0x8239,				// GL_RG16I 
		RG16UI = 0x823A,			// GL_RG16UI
		RG32I = 0x823B,				// GL_RG32I 
		RG32UI = 0x823C,			// GL_RG32UI
		RGBA32F = 0x881,			// GL_RGBA32F
		RGBA16F = 0x881,			// GL_RGBA16F
		RGBA32UI = 0x8D70,			// GL_RGBA32UI

		DEPTH_COMPONENT16 = 0x81A5,
		DEPTH_COMPONENT24 = 0x81A6,
		DEPTH_COMPONENT32F = 0x81A7,
		DEPTH24_STENCIL8 = 0x88F0,
		DEPTH32F_STENCIL8 = 0x8CAD,
		STENCIL_INDEX8 = 0x8D48
	};

	RenderBuffer(InternalFormat internal_format, unsigned int width, unsigned int height);
	RenderBuffer(unsigned int samples, InternalFormat internal_format, unsigned int width, unsigned int height); // Multisample
	~RenderBuffer();

	void bind() const;
	void unbind() const;

	void respecify_rbo(InternalFormat internal_format, unsigned int width, unsigned int height); // delete rbo and recreate it.
	void respecify_rbo(unsigned int samples, InternalFormat internal_format, unsigned int width, unsigned int height);
	inline unsigned int get_rbo_id() const { return m_rbo; }

private:
	unsigned int m_rbo;
};
