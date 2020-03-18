#pragma once

#include "Foundation/Uncopyable.h"
#include "Foundation/Math/Vector.h"

class BasicTexture2D : public Uncopyable
{
public:
	enum TextureFilter
	{
		NEAREST = 0x2600, // GL_NEAREST. 
		LINEAR = 0x2601, // GL_LINEAR. - Bilinear filtering 
		NEAREST_MIPMAP_NEAREST = 0x2700, // GL_NEAREST_MIPMAP_NEAREST.
		LINEAR_MIPMAP_NEAREST = 0x2701, // GL_LINEAR_MIPMAP_NEAREST. - linear filtering between mipmap levels, but no filtering inside each mipmap level.
		NEAREST_MIPMAP_LINEAR = 0x2702, // GL_NEAREST_MIPMAP_LINEAR. - bilinear filtering.
		LINEAR_MIPMAP_LINEAR = 0x2703 // GL_LINEAR_MIPMAP_LINEAR. - trilinear filtering.
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
		RGBA32F = 0x881,			// GL_RGBA32F
		RGBA32UI = 0x8D70,			// GL_RGBA32UI
		RGB32F = 0x8815,			// GL_RGB32F
		RGBA16F = 0x881,			// GL_RGBA16F
		RGB16F = 0x881B,			// GL_RGB16F
	};
	enum Format
	{
		FORMAT_DEPTH_COMPONENT = 0x1902,	// GL_DEPTH_CO
		FORMAT_DEPTH_STENCIL = 0x84F9,		// GL_DEPTH_ST
		FORMAT_RED = 0x1903,				// GL_RED
		FORMAT_RG = 0x8227,					// GL_RG,
		FORMAT_RGB = 0x1907,				// GL_RGB
		FORMAT_RGBA = 0x1908,				// GL_RGBA
		FORMAT_RGBA_INTEGER = 0x8D99		// GL_RGBA_INTEGER
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
	struct TextureParm;

	BasicTexture2D(TextureParm const& parm);
	~BasicTexture2D();

	void respecify_textute(TextureParm const& parm) const;
	void gen_mipmaps() const;

	void bind(unsigned int slot) const;
	void unbind() const;

	inline unsigned int get_texture() const { return m_texture; }

	static unsigned int get_max_mipmap_level(Vector2i size) { return std::log2f(static_cast<float>(std::max(size[0], size[1]))); }

private:
	void gen_texture(TextureParm const& parm);

	unsigned int m_texture;
};

struct BasicTexture2D::TextureParm
{
	constexpr TextureParm(TextureFilter const mim_filter, TextureFilter const mag_filter, TextureWrap const s,
		TextureWrap const t, int const detail_level, InternalFormat const internal_format, Vector2i const& res, Format const format, DataType const type, void* data);
	~TextureParm() = default;

	TextureFilter m_mim_filter;
	TextureFilter m_mag_filter;
	TextureWrap m_s;
	TextureWrap m_t;
	int m_detail_level;
	InternalFormat m_internal_format;
	Vector2i m_resolution;
	Format m_format;
	DataType m_type;
	void* m_data;
};
