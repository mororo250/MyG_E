#include "ShadowMap.h"

#include "Core/Post-Processing/GaussianBlur.h"
#include "Core/Post-Processing/SummedAreaTable.h"
#include "Core/Shader.h"
#include "Core/Renderer3D.h"
#include "Foundation/Gldebug.h"
#include "Foundation/Game.h"

#include "glad/glad.h"
#include "imgui.h"

BasicTexture2D::TextureParm ShadowMap::s_parm = BasicTexture2D::TextureParm(BasicTexture2D::NEAREST, BasicTexture2D::NEAREST, BasicTexture2D::CLAMP_TO_BORDER,
	BasicTexture2D::CLAMP_TO_BORDER, 0, BasicTexture2D::RGBA32UI, Vector2i(1024, 1024), BasicTexture2D::FORMAT_RGBA_INTEGER, BasicTexture2D::UNSIGNED_INT, nullptr);
int ShadowMap::s_number_msaa = 4;
int ShadowMap::s_shadow_softness = 5;

ShadowMap::ShadowMap(Camera const* camera, unsigned int csm_layers)
	: m_camera_view(camera)
	, m_fbo_msaa()
	, m_fbo()
	, m_depth_rbo(s_number_msaa, RenderBuffer::DEPTH_COMPONENT, s_parm.m_resolution[0], s_parm.m_resolution[1])
	, m_sat(new BasicTexture2D(s_parm), s_parm.m_resolution)
{
	for (unsigned int i = 0; i < csm_layers; i++)
	{
		m_textures.push_back(new BasicTexture2D(s_parm));
		m_textures[i]->bind(0);
		static float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		m_textures[i]->unbind();
		
		m_color_rbos.push_back(new RenderBuffer(s_number_msaa, RenderBuffer::RG32UI, s_parm.m_resolution[0], s_parm.m_resolution[1]));
	}
	m_fbo_msaa.bind();
	m_fbo_msaa.attach_rbo(m_depth_rbo, FrameBuffer::DEPTH_ATTACHMENT);
	for (unsigned int i = 0; i < csm_layers; i++)
		m_fbo_msaa.attach_rbo(*m_color_rbos[i], FrameBuffer::Attachment(FrameBuffer::COLOR_ATTACHMENT0 + i));

	m_fbo_msaa.check_status();
	GLcall(glEnable(GL_MULTISAMPLE));

	m_fbo.bind();
	for (unsigned int i = 0; i < csm_layers; i++)
		m_fbo.attach_texture(*m_textures[i], FrameBuffer::Attachment(FrameBuffer::COLOR_ATTACHMENT0 + i));
	m_fbo.check_status();
	m_fbo.unbind();
}


ShadowMap::~ShadowMap() 
{
}

void ShadowMap::begin() const
{
	// Often shadow maps has a different resolution compared with what we originally render the scene.
	GLcall(glViewport(0, 0, s_parm.m_resolution[0], s_parm.m_resolution[1]));
	m_fbo_msaa.bind();
}

void ShadowMap::end() const
{
	m_fbo_msaa.unbind();
	GLcall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo_msaa.get_fbo_id()));
	GLcall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo.get_fbo_id()));
	GLcall(glBlitFramebuffer(0, 0,  s_parm.m_resolution[0], s_parm.m_resolution[1], 0, 0, s_parm.m_resolution[0],
		s_parm.m_resolution[1], GL_COLOR_BUFFER_BIT, GL_NEAREST));

	// Gen Summed Area Table
	for(unsigned int i = 0; i < m_textures.size(); i++)
		m_sat.gen_sat(*m_textures[i]);

	// Set viewport back to normal.
	Vector2i window_size = Game::Get().get_window_size();
	GLcall(glViewport(0, 0, window_size[0], window_size[1]));
}

void ShadowMap::bind(unsigned int const slot) const
{
	for (unsigned int i = 0; i < m_textures.size(); i++)
		m_textures[i]->bind(slot + i);
}

void ShadowMap::unbind() const
{
	m_textures[0]->unbind();
}

void ShadowMap::imgui_renderer()
{	
	static const BasicTexture2D tex(BasicTexture2D::TextureParm(BasicTexture2D::LINEAR, BasicTexture2D::LINEAR, BasicTexture2D::CLAMP_TO_BORDER, BasicTexture2D::CLAMP_TO_BORDER,
		0, BasicTexture2D::RGBA, Vector2i(256, 256), BasicTexture2D::FORMAT_RGBA, BasicTexture2D::UNSIGNED_BYTE, nullptr));
	static Shader const shader("BasicShader.vert", "ShadowMapDebug.frag");
	static FrameBuffer const fbo;
	
	fbo.bind();
	fbo.attach_texture(tex, FrameBuffer::COLOR_ATTACHMENT0);

	// Draw shadow maps.
	shader.bind();
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		m_textures[i]->bind(i);
		shader.set_uniform1i(shader.get_uniform_location(std::string("u_shadow_map") +  "[" + std::to_string(i) + "]"), i);
		GLcall(glViewport(0, 0, 256, 256));
		Renderer3D::draw_quad();
	}
	Vector2i window_size = Game::Get().get_window_size();
	GLcall(glViewport(0, 0, window_size[0], window_size[1]));
	shader.unbind();
	m_textures[0]->unbind();
	fbo.unbind();
	
	// Resolution
	Vector2i temp_res = s_parm.m_resolution;
	int temp_msaa = s_number_msaa;
	ImGui::DragInt("Shadow Softness", &s_shadow_softness, 1, 1, 128);
	ImGui::DragInt2("Shadow resolution", s_parm.m_resolution.get_as_pointer(), 1, 256, 8192);
	ImGui::DragInt("Shadow MSAA", &s_number_msaa, 1.0, 0, 32);
	if (temp_res != s_parm.m_resolution || temp_msaa != s_number_msaa)
	{
		m_fbo_msaa.bind();
		m_depth_rbo.respecify_rbo(s_number_msaa, RenderBuffer::DEPTH_COMPONENT, s_parm.m_resolution[0], s_parm.m_resolution[1]);
		m_fbo_msaa.attach_rbo(m_depth_rbo, FrameBuffer::DEPTH_ATTACHMENT);
		for (unsigned int i = 0; i < m_color_rbos.size(); i++)
		{
			m_color_rbos[i]->respecify_rbo(s_number_msaa, RenderBuffer::RGBA32UI, s_parm.m_resolution[0], s_parm.m_resolution[1]);
			m_fbo_msaa.attach_rbo(m_depth_rbo, FrameBuffer::Attachment(FrameBuffer::COLOR_ATTACHMENT0 + i));
		}
		m_fbo_msaa.unbind();
		for (unsigned int i = 0; i < m_color_rbos.size(); i++)
			m_textures[i]->respecify_textute(s_parm);
	}

	ImGui::Indent(25.0f);
	ImGui::Image(reinterpret_cast<unsigned int*>(tex.get_texture()), ImVec2(256, 256), ImVec2(0, 0), ImVec2(1, 1));
}
