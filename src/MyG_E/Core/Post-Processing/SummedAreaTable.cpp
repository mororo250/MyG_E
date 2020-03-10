#include "SummedAreaTable.h"

#include "Core/FrameBuffer.h"
#include "Core/Shader.h"
#include "Core/Renderer3D.h"
#include"Core/Texture/BasicTexture2D.h"

#include "glad/glad.h"

void gen_SAT(BasicTexture2D& tex)
{
	static Shader horizontal_sta("BasicShader.vert", "SummedAreaTableHorizontal.frag");
	static Shader vertical_sta("BasicShader.vert", "SummedAreaTableVertical.frag");
	static Shader basic_texture_shader("BasicShader.vert", "DrawTexture.frag");

	static BasicTexture2D aux_texture(BasicTexture2D::NEAREST, BasicTexture2D::NEAREST, BasicTexture2D::CLAMP_TO_BORDER, BasicTexture2D::CLAMP_TO_BORDER,
		0, BasicTexture2D::RG32F, 64, 64, BasicTexture2D::FORMAT_RG, BasicTexture2D::FLOAT, nullptr);
	static BasicTexture2D* texture_pointer;
	static FrameBuffer fbo;

	aux_texture.bind(0);
	static float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	aux_texture.unbind();

	float nx = log2f(64);
	float ny = log2f(64);

	fbo.bind();
	vertical_sta.bind();
	texture_pointer = &aux_texture;
	tex.bind(0);

	for (float i = 0.0f; i < ny; i++)
	{
		fbo.detach_texture(FrameBuffer::COLOR_ATTACHMENT);
		fbo.attach_texture(*texture_pointer, FrameBuffer::COLOR_ATTACHMENT);
		vertical_sta.set_uniform1i(vertical_sta.get_uniform_location("u_texture"), 0);
		vertical_sta.set_uniform1f(vertical_sta.get_uniform_location("u_offset"), std::pow(2.0, i));
		Renderer3D::draw_quad();
		if (texture_pointer == &aux_texture)
		{
			texture_pointer = &tex;
			aux_texture.bind(0);
		}
		else
		{
			texture_pointer = &aux_texture;
			tex.bind(0);
		}
	}
	horizontal_sta.bind();

	for (float i = 0.0f; i < nx; i++)
	{
		fbo.detach_texture(FrameBuffer::COLOR_ATTACHMENT);
		fbo.attach_texture(*texture_pointer, FrameBuffer::COLOR_ATTACHMENT);
		horizontal_sta.set_uniform1i(horizontal_sta.get_uniform_location("u_texture"), 0);
		horizontal_sta.set_uniform1f(horizontal_sta.get_uniform_location("u_offset"), std::pow(2.0, i));
		Renderer3D::draw_quad();
		if (texture_pointer == &aux_texture)
		{
			texture_pointer = &tex;
			aux_texture.bind(0);
		}
		else
		{
			texture_pointer = &aux_texture;
			tex.bind(0);
		}
	}
	horizontal_sta.unbind();
	if (texture_pointer == &tex)
	{
		static int const loc = basic_texture_shader.get_uniform_location("u_texture");
		basic_texture_shader.bind();
		aux_texture.bind(0);
		basic_texture_shader.set_uniform1i(loc, 0);
		Renderer3D::draw_quad();
		basic_texture_shader.unbind();
		aux_texture.unbind();
	}

	fbo.unbind();
}
