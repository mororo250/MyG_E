#include "ShadowMap.h"

#include "Foundation/Gldebug.h"
#include "Foundation/Game.h"
#include "Core/Shader.h"
#include "Core/Renderer3D.h"

#include "glad/glad.h"
#include "imgui.h"

ShadowMap::ShadowMap(Camera const* camera)
	: m_camera_view(camera)
	, m_texture(BasicTexture2D::NEAREST, BasicTexture2D::NEAREST, BasicTexture2D::CLAMP_TO_BORDER, BasicTexture2D::CLAMP_TO_BORDER,
		0, BasicTexture2D::DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, BasicTexture2D::DEPTH_COMPONENT, BasicTexture2D::FLOAT, nullptr)
	,m_depth_map(m_texture.get_texture(), FrameBuffer::DEPTH_ATTACHMENT)
{
	m_depth_map.disable_color_buffer();

	m_depth_map.unbind();
	unbind();
}

ShadowMap::~ShadowMap() 
{
}

void ShadowMap::begin() const
{
	// Often shadow maps has a different resolution compared with what we originally render the scene.
	GLcall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
	m_depth_map.bind();
}

void ShadowMap::end() const
{
	// Set viewport back to normal.
	std::pair<int, int> window_size = Game::Get().get_window_size();
	GLcall(glViewport(0, 0, window_size.first, window_size.second));
	m_depth_map.unbind();
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
	static BasicTexture2D const tex(BasicTexture2D::LINEAR, BasicTexture2D::LINEAR, BasicTexture2D::REPEAT, BasicTexture2D::REPEAT,
		0, BasicTexture2D::RGBA, SHADOW_WIDTH, SHADOW_HEIGHT, BasicTexture2D::RGBA, BasicTexture2D::UNSIGNED_BYTE, nullptr);
	static Shader const shader("ShadowMapDebug.vert", "ShadowMapDebug.frag");
	static int const loc = shader.get_uniform_location("u_shadow_map");
	static FrameBuffer const fbo(tex.get_texture(), FrameBuffer::COLOR_ATTACHMENT);
	
	shader.bind();
	fbo.bind();
	m_texture.bind(0);
	shader.set_uniform1i(loc, 0);
	Renderer3D::draw_quad();
	fbo.unbind();
	shader.unbind();
	tex.unbind();

	ImGui::Indent(10.0f); 
	ImGui::Image(reinterpret_cast<unsigned int*>(tex.get_texture()), ImVec2(256, 256), ImVec2(0, 0), ImVec2(1, 1));
}
