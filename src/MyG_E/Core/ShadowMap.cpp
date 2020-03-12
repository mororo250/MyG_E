#include "ShadowMap.h"

#include "Core/Post-Processing/GaussianBlur.h"
#include "Core/Post-Processing/SummedAreaTable.h"
#include "Core/Shader.h"
#include "Core/Renderer3D.h"
#include "Foundation/Gldebug.h"
#include "Foundation/Game.h"

#include "glad/glad.h"
#include "imgui.h"

Vector2i ShadowMap::s_shadow_resolution = { 1024, 1024 };
int ShadowMap::s_number_msaa = 4;

ShadowMap::ShadowMap(Camera const* camera)
	: m_camera_view(camera)
	, m_texture(BasicTexture2D::NEAREST, BasicTexture2D::NEAREST, BasicTexture2D::CLAMP_TO_BORDER, BasicTexture2D::CLAMP_TO_BORDER,
		0, BasicTexture2D::RGBA32UI, s_shadow_resolution[0], s_shadow_resolution[1], BasicTexture2D::FORMAT_RGBA_INTEGER, BasicTexture2D::UNSIGNED_INT, nullptr)
	, m_fbo_msaa()
	, m_fbo()
	, m_rbo_depth(s_number_msaa, RenderBuffer::DEPTH_COMPONENT, s_shadow_resolution[0], s_shadow_resolution[1])
	, m_rbo_color(s_number_msaa, RenderBuffer::RGBA32UI, s_shadow_resolution[0], s_shadow_resolution[1])
	, m_sat(&m_texture, s_shadow_resolution)
{
	m_fbo_msaa.bind();
	m_fbo_msaa.attach_rbo(m_rbo_depth, FrameBuffer::DEPTH_ATTACHMENT);
	m_fbo_msaa.attach_rbo(m_rbo_color, FrameBuffer::COLOR_ATTACHMENT0);
	m_fbo_msaa.check_status();
	GLcall(glEnable(GL_MULTISAMPLE));
	m_fbo.bind();
	m_fbo.attach_texture(m_texture, FrameBuffer::COLOR_ATTACHMENT0);
	m_fbo.check_status();
	m_fbo.unbind();

	//Borders are unsupported on a whole range of hardware
	m_texture.bind(0);
	static float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	m_texture.unbind();
}


ShadowMap::~ShadowMap() 
{
}

void ShadowMap::begin() const
{
	// Often shadow maps has a different resolution compared with what we originally render the scene.
	GLcall(glViewport(0, 0, s_shadow_resolution[0], s_shadow_resolution[1]));
	m_fbo_msaa.bind();
}

void ShadowMap::end() const
{
	static SummeadAreaTable sat(&m_texture, s_shadow_resolution);

	m_fbo_msaa.unbind();
	GLcall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo_msaa.get_fbo_id()));
	GLcall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo.get_fbo_id()));
	GLcall(glBlitFramebuffer(0, 0, s_shadow_resolution[0], s_shadow_resolution[1], 0, 0, s_shadow_resolution[0],
		s_shadow_resolution[1], GL_COLOR_BUFFER_BIT, GL_NEAREST));

	// Gen Summed Area Table
	sat.gen_sat(s_shadow_resolution);

	// Set viewport back to normal.
	Vector2i window_size = Game::Get().get_window_size();
	GLcall(glViewport(0, 0, window_size[0], window_size[1]));
}

void ShadowMap::bind(unsigned int const slot) const
{
	m_texture.bind(slot);
}

void ShadowMap::unbind() const
{
	m_texture.unbind();
}

void ShadowMap::imgui_renderer()
{	
	static const BasicTexture2D tex(BasicTexture2D::LINEAR, BasicTexture2D::LINEAR, BasicTexture2D::CLAMP_TO_BORDER, BasicTexture2D::CLAMP_TO_BORDER,
		0, BasicTexture2D::RGBA, 256, 256, BasicTexture2D::FORMAT_RGBA, BasicTexture2D::UNSIGNED_BYTE, nullptr);
	static Shader const shader("BasicShader.vert", "ShadowMapDebug.frag");
	static int const loc = shader.get_uniform_location("u_shadow_map");
	static FrameBuffer const fbo;
	
	fbo.bind();
	fbo.attach_texture(tex, FrameBuffer::COLOR_ATTACHMENT0);

	// Draw shadow map.
	shader.bind();
	m_texture.bind(0);
	shader.set_uniform1i(loc, 0);
	GLcall(glViewport(0, 0, 256, 256));
	Renderer3D::draw_quad();
	Vector2i window_size = Game::Get().get_window_size();
	GLcall(glViewport(0, 0, window_size[0], window_size[1]));
	shader.unbind();
	m_texture.unbind();
	fbo.unbind();

	// Resolution
	Vector2i temp_res = s_shadow_resolution;
	int temp_msaa = s_number_msaa;
	ImGui::DragInt2("Shadow map resolution: ", s_shadow_resolution.get_as_pointer(), 1, 256, 8192);
	ImGui::DragInt("Shadow Map MSAA filtering:", &s_number_msaa, 1.0, 0, 32);
	if (temp_res != s_shadow_resolution || temp_msaa != s_number_msaa)
	{
		m_fbo_msaa.bind();
		m_rbo_depth.respecify_rbo(s_number_msaa, RenderBuffer::DEPTH_COMPONENT, s_shadow_resolution[0], s_shadow_resolution[1]);
		m_rbo_color.respecify_rbo(s_number_msaa, RenderBuffer::RGBA32UI, s_shadow_resolution[0], s_shadow_resolution[1]);
		m_fbo_msaa.attach_rbo(m_rbo_depth, FrameBuffer::DEPTH_ATTACHMENT);
		m_fbo_msaa.attach_rbo(m_rbo_color, FrameBuffer::COLOR_ATTACHMENT0);
		m_fbo_msaa.unbind();
		m_texture.respecify_textute(0, BasicTexture2D::RGBA32UI, s_shadow_resolution[0], s_shadow_resolution[1],
			BasicTexture2D::FORMAT_RGBA_INTEGER, BasicTexture2D::UNSIGNED_INT, nullptr);
	}

	ImGui::Indent(25.0f);
	ImGui::Image(reinterpret_cast<unsigned int*>(tex.get_texture()), ImVec2(256, 256), ImVec2(0, 0), ImVec2(1, 1));
}
