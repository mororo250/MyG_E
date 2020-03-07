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
		ImGui::DragInt("Kernel Size", &m_kernel_size, 1, 0, 15);
		ImGui::DragFloat("Sigma", &m_sigma, 0.1f, 0.0f, 7.0f);
	}
}

void GaussianBlur::apply_filter(BasicTexture2D const& input_texture)
{
	static Shader s_horizontal_blur("BasicShader.vert", "GaussianBlurHorizontal.frag");
	static Shader s_vertical_blur("BasicShader.vert", "GaussianBlurVertical.frag");

	BasicTexture2D aux_texture(BasicTexture2D::LINEAR, BasicTexture2D::LINEAR, BasicTexture2D::CLAMP_TO_EDGE, 
		BasicTexture2D::CLAMP_TO_EDGE, 0, BasicTexture2D::RGBA, Game::Get().get_window_size()[0], 
		Game::Get().get_window_size()[1], BasicTexture2D::FORMAT_RGBA, BasicTexture2D::FLOAT, nullptr);
	FrameBuffer fbo(aux_texture.get_texture(), FrameBuffer::COLOR_ATTACHMENT);
	
	// Horizontal blur.
	fbo.bind();
	s_horizontal_blur.bind();
	input_texture.bind(0);
	s_horizontal_blur.set_uniform1i(s_horizontal_blur.get_uniform_location("u_texture"), 0);
	s_horizontal_blur.set_uniform1ui(s_horizontal_blur.get_uniform_location("u_kernel_size"), m_kernel_size);
	s_horizontal_blur.set_uniform1f(s_horizontal_blur.get_uniform_location("u_sigma"), m_sigma);
	Renderer3D::draw_quad();
	input_texture.unbind();
	s_horizontal_blur.unbind();

	// Vertical blur.
	fbo.change_texture(input_texture.get_texture(), FrameBuffer::COLOR_ATTACHMENT);
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
