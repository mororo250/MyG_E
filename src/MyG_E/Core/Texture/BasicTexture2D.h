#pragma once

class BasicTexture2D
{
public:

	enum TextureFilter
	{
		NEAREST = 0x2600, // GL_NEAREST
		LINEAR = 0x2601 // GL_LINEAR
	};
	enum TextureWrap
	{
		CLAMP_TO_EDGE = 0x812F, // GL_CLAMP_TO_EDGE,
		MIRROR_CLAMP_TO_EDGE = 0x8743, // GL_MIRROR_CLAMP_TO_EDGE
		CLAMP_TO_BORDER = 0x812D, // GL_CLAMP_TO_BORDER
		MIRRORED_REPEA = 0x8370, // GL_MIRRORED_REPEAT
		REPEAT = 0x2901, // GL_REPEAT
	};
	enum InternalFormat
	{
		DEPTH_COMPONENT = 0x1902, // GL_DEPTH_COMPONENT
		DEPTH_STENCIL = 0x84F9, // GL_DEPTH_STENCIL
		RED = 0x1903, // GL_RED
		RG = 0x8227, // GL_RG,
		RGB = 0x1907, // GL_RGB,
		RGBA = 0x1908 // GL_RGBA
	};
	enum DataType
	{
		BYTE = 0x1400, // GL_BYTE,
		UNSIGNED_BYTE = 0x1401, // GL_UNSIGNED_BYTE
		SHORT = 0x1402, // GL_SHORT
		UNSIGNED_SHORT = 0x1403, // GL_UNSIGNED_SHORT
		INT = 0x1404, // GL_INT
		UNSIGNED_INT = 0x1405, // GL_UNSIGNED_INT
		FLOAT = 0x1406 // GL_FLOAT
	};

	BasicTexture2D(TextureFilter const mim_filter, TextureFilter const mag_filter, TextureWrap const s, 
		TextureWrap const t, int const detail_level, InternalFormat const internal_format, int const width,
		int const height, InternalFormat const format, DataType type, void* data);

	~BasicTexture2D();

	void bind(unsigned int slot) const;
	void unbind() const;

	inline unsigned int get_texture() const { return m_texture; }

private:
	unsigned int m_texture;
};