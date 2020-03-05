#include "ShadowMap.h"

#include "Foundation/Gldebug.h"
#include "Foundation/Game.h"
#include "Core/Shader.h"
#include "Core/Renderer3D.h"

#include "glad/glad.h"
#include "imgui.h"

Vector2i ShadowMap::s_shadow_resolution = { 1024, 1024 };

ShadowMap::ShadowMap(Camera const* camera)
	: m_camera_view(camera)
	, m_texture(BasicTexture2D::LINEAR_MIPMAP_LINEAR, BasicTexture2D::LINEAR, BasicTexture2D::CLAMP_TO_EDGE, BasicTexture2D::CLAMP_TO_EDGE
		, 0, BasicTexture2D::RG32F, s_shadow_resolution[0], s_shadow_resolution[1],BasicTexture2D::FORMAT_RG, BasicTexture2D::FLOAT, nullptr)
	, m_fbo(m_texture.get_texture(), FrameBuffer::COLOR_ATTACHMENT)
	, m_rbo(RenderBuffer::DEPTH_COMPONENT, s_shadow_resolution[0], s_shadow_resolution[1])
{
	m_fbo.bind();
	m_fbo.attach_rbo(FrameBuffer::DEPTH_ATTACHMENT, m_rbo);
	/*
	Borders are unsupported on a whole range of hardware
	m_texture.bind(0);
	//static float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	*/
	m_texture.gen_mipmaps(); // ALocate memory of all textures levels
	m_texture.unbind();
	m_fbo.unbind();
}


ShadowMap::~ShadowMap() 
{
}

void ShadowMap::begin() const
{
	// Often shadow maps has a different resolution compared with what we originally render the scene.
	GLcall(glViewport(0, 0, s_shadow_resolution[0], s_shadow_resolution[1]));
	m_fbo.bind();
}

void ShadowMap::end() const
{
	// Set viewport back to normal.
	Vector2i window_size = Game::Get().get_window_size();
	GLcall(glViewport(0, 0, window_size[0], window_size[1]));
	m_fbo.unbind();

	// Generate mipmaps
	m_texture.bind(0);
	m_texture.gen_mipmaps(); // Fill smaller levels with downsampled images.
	m_texture.unbind();
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
	static const BasicTexture2D tex(BasicTexture2D::LINEAR, BasicTexture2D::LINEAR, BasicTexture2D::REPEAT, BasicTexture2D::REPEAT,
		0, BasicTexture2D::RGBA, 256, 256, BasicTexture2D::FORMAT_RGBA, BasicTexture2D::UNSIGNED_BYTE, nullptr);
	static Shader const shader("BasicShader.vert", "ShadowMapDebug.frag");
	static int const loc = shader.get_uniform_location("u_shadow_map");
	static FrameBuffer const fbo(tex.get_texture(), FrameBuffer::COLOR_ATTACHMENT);

	// Draw shadow map.
	shader.bind();
	fbo.bind();
	m_texture.bind(0);
	shader.set_uniform1i(loc, 0);
	GLcall(glViewport(0, 0, 256, 256));
	Renderer3D::draw_quad();
	Vector2i window_size = Game::Get().get_window_size();
	GLcall(glViewport(0, 0, window_size[0], window_size[1]));
	fbo.unbind();
	shader.unbind();
	m_texture.unbind();

	// Resolution
	Vector2i temp = s_shadow_resolution;
	ImGui::DragInt2("Shadow map resolution: ", s_shadow_resolution.get_as_pointer(), 1, 256, 8192);
	if (temp != s_shadow_resolution)
	{
		m_fbo.bind();
		m_fbo.detach_rbo(FrameBuffer::DEPTH_ATTACHMENT);
		m_rbo.respecify_rbo(RenderBuffer::DEPTH_COMPONENT, s_shadow_resolution[0], s_shadow_resolution[1]);
		m_fbo.attach_rbo(FrameBuffer::DEPTH_ATTACHMENT, m_rbo);
		m_fbo.unbind();
		m_texture.respecify_textute(0, BasicTexture2D::RG32F, s_shadow_resolution[0]
			, s_shadow_resolution[1], BasicTexture2D::FORMAT_RG, BasicTexture2D::FLOAT, nullptr); // Resize texture.
	}

	ImGui::Indent(10.0f);
	ImGui::Image(reinterpret_cast<unsigned int*>(tex.get_texture()), ImVec2(256, 256), ImVec2(0, 0), ImVec2(1, 1));
}
