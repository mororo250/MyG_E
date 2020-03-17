#include "SummedAreaTable.h"

#include "Core/FrameBuffer.h"
#include "Core/Shader.h"
#include "Core/Renderer3D.h"
#include"Core/Texture/BasicTexture2D.h"

#include "glad/glad.h"

SummeadAreaTable::SummeadAreaTable(BasicTexture2D* sat_texture, Vector2i const& texture_resolution)
	: m_sat_texture(sat_texture)
	, m_fbo()
	, m_texture_resolution(texture_resolution)
{
	m_fbo.bind();
	m_fbo.attach_texture(*m_sat_texture.get(), FrameBuffer::COLOR_ATTACHMENT1);
	m_fbo.check_status();
	m_fbo.unbind();
}

void SummeadAreaTable::gen_sat(BasicTexture2D const& input_texture)
{
	static Shader horizontal_sta("BasicShader.vert", "SummedAreaTableHorizontal.frag");
	static Shader vertical_sta("BasicShader.vert", "SummedAreaTableVertical.frag");
	static Shader basic_texture_shader("BasicShader.vert", "DrawTexture.frag");

	float nx = log2f(m_texture_resolution[0]) / log2f(4.0f);
	float ny = log2f(m_texture_resolution[1]) / log2f(4.0f);
	int current_buffer = FrameBuffer::COLOR_ATTACHMENT1;

	m_fbo.bind();
	m_fbo.attach_texture(*m_sat_texture.get(), FrameBuffer::COLOR_ATTACHMENT0);
	glDrawBuffer(current_buffer);
	Renderer3D::clear_color_buffer({0.0f, 0.0f, 0.0f, 0.0f}); // Clear m_sat_texture buffer.
	vertical_sta.bind();
	input_texture.bind(0);
	for (float i = 0.0f; i < ny; i++)
	{
		vertical_sta.set_uniform1i(vertical_sta.get_uniform_location("u_texture"), 0);
		vertical_sta.set_uniform1f(vertical_sta.get_uniform_location("u_offset"), std::pow(4.0, i));
		Renderer3D::draw_quad();
		if (current_buffer == FrameBuffer::COLOR_ATTACHMENT1)
		{
			current_buffer = FrameBuffer::COLOR_ATTACHMENT0;
			glDrawBuffer(current_buffer);
			m_sat_texture->bind(0);
		}
		else
		{
			current_buffer = FrameBuffer::COLOR_ATTACHMENT1;
			glDrawBuffer(current_buffer);
			input_texture.bind(0);
		}
	}
	horizontal_sta.bind();

	for (float i = 0.0f; i < nx; i++)
	{
		horizontal_sta.set_uniform1i(horizontal_sta.get_uniform_location("u_texture"), 0);
		horizontal_sta.set_uniform1f(horizontal_sta.get_uniform_location("u_offset"), std::pow(4.0, i));
		Renderer3D::draw_quad();
		if (current_buffer == FrameBuffer::COLOR_ATTACHMENT1)
		{
			current_buffer = FrameBuffer::COLOR_ATTACHMENT0;
			glDrawBuffer(current_buffer);
			m_sat_texture->bind(0);
		}
		else
		{
			current_buffer = FrameBuffer::COLOR_ATTACHMENT1;
			glDrawBuffer(current_buffer);
			input_texture.bind(0);
		}
	}
	horizontal_sta.unbind();

	
	if (current_buffer == FrameBuffer::COLOR_ATTACHMENT0)
	{
		basic_texture_shader.bind();
		static int const loc = basic_texture_shader.get_uniform_location("u_texture");
		m_sat_texture->bind(0);
		basic_texture_shader.set_uniform1i(loc, 0);
		Renderer3D::draw_quad();
		basic_texture_shader.unbind();
		m_sat_texture->unbind();
	}
	m_fbo.detach_rbo(FrameBuffer::COLOR_ATTACHMENT0);
	m_fbo.unbind();
}
