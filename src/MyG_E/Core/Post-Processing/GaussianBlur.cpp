#include "GaussianBlur.h"

#include "imgui.h"

#include "Core/FrameBuffer.h"
#include "Core/Renderer3D.h"
#include "Core/Texture/BasicTexture2D.h"
#include "Foundation/Game.h"

GaussianBlur::GaussianBlur(int const kernel_size, float const sigma)
	: m_kernel_size(kernel_size)
	, m_sigma(sigma)
{
}

void GaussianBlur::imgui_renderer()
{
	if (ImGui::CollapsingHeader("Gaussian BLur", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragInt("Kernel Size", &m_kernel_size, 1, 0, 30);
		ImGui::DragFloat("Sigma", &m_sigma, 0.1f, 0.0f, 30.0f);
	}
}

void GaussianBlur::apply_filter(BasicTexture2D& input_texture)
{
	static Shader s_horizontal_blur("BasicShader.vert", "GaussianBlurHorizontal.frag");
	static Shader s_vertical_blur("BasicShader.vert", "GaussianBlurVertical.frag");

	static BasicTexture2D aux_texture(BasicTexture2D::NEAREST, BasicTexture2D::NEAREST, BasicTexture2D::CLAMP_TO_EDGE, BasicTexture2D::CLAMP_TO_EDGE,
		0, BasicTexture2D::RG32F, 1024, 1024, BasicTexture2D::FORMAT_RG, BasicTexture2D::FLOAT, nullptr);
	static FrameBuffer fbo;
	
	fbo.bind();
	Renderer3D::clear();
	fbo.attach_texture(aux_texture, FrameBuffer::COLOR_ATTACHMENT0);
	fbo.check_status();
	
	// Horizontal blur.
	s_horizontal_blur.bind();
	input_texture.bind(0);
	s_horizontal_blur.set_uniform1i(s_horizontal_blur.get_uniform_location("u_texture"), 0);
	s_horizontal_blur.set_uniform1ui(s_horizontal_blur.get_uniform_location("u_kernel_size"), m_kernel_size);
	s_horizontal_blur.set_uniform1f(s_horizontal_blur.get_uniform_location("u_sigma"), m_sigma);
	Renderer3D::draw_quad();
	input_texture.unbind();
	s_horizontal_blur.unbind();

	// Vertical blur.
	fbo.detach_texture(FrameBuffer::COLOR_ATTACHMENT0);
	fbo.attach_texture(input_texture, FrameBuffer::COLOR_ATTACHMENT0);
	s_vertical_blur.bind();
	aux_texture.bind(0);
	s_vertical_blur.set_uniform1i(s_vertical_blur.get_uniform_location("u_texture"), 0);
	s_vertical_blur.set_uniform1ui(s_vertical_blur.get_uniform_location("u_kernel_size"), m_kernel_size);
	s_vertical_blur.set_uniform1f(s_vertical_blur.get_uniform_location("u_sigma"), m_sigma);
	Renderer3D::draw_quad();
	aux_texture.unbind();
	s_vertical_blur.unbind();
	fbo.unbind();
}
