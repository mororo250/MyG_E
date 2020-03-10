#pragma once

#include "Foundation/Uncopyable.h"

class Texture2DMultisample : public Uncopyable
{
public:
	enum InternalFormat
	{
		DEPTH_COMPONENT = 0x1902,	// GL_DEPTH_COMPONENT
		DEPTH_STENCIL = 0x84F9,		// GL_DEPTH_STENCIL
		RED = 0x1903,				// GL_RED
		RG = 0x8227,				// GL_RG,
		RGB = 0x1907,				// GL_RGB
		RGBA = 0x1908,				// GL_RGBA

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
	};

	Texture2DMultisample(unsigned int const num_samples, InternalFormat const internal_format, int const width,
		int const height, void* data);
	~Texture2DMultisample();

	void respecify_textute(unsigned int const num_samples, InternalFormat const internal_format, int const width,
		int const height, void* data) const;
	inline unsigned int get_texture() const { return m_texture; }

	void bind(unsigned int slot) const;
	void unbind() const;

private:

	unsigned int m_texture;
};