#pragma once

#include "Foundation/Math/Vector.h"
#include "Core/Post-Processing/SummedAreaTable.h"
#include "Core/Texture/BasicTexture2D.h"
#include "Core/FrameBuffer.h"
#include "Core/RenderBuffer.h"
#include "Core/Camera/Camera.h"

class ShadowMap
{
public:
	explicit ShadowMap(Camera const* camera, unsigned int csm_layers);
	~ShadowMap();

	void begin() const; // call before draw buffer
	void end() const; // call after draw buffer

	void bind(unsigned int const slot = 0) const; // bind texture
	void unbind() const; // unbind texture

	inline Vector3f get_central_pos() const { return m_camera_view->get_position() + (SHADOW_RADIUS * m_camera_view->get_direction()); }
	inline Camera const* get_camera_view() { return m_camera_view; }
	inline static constexpr float get_shadow_radius() { return SHADOW_RADIUS; }
	inline static int get_shadow_softness() { return s_shadow_softness; }

	void imgui_renderer();

private:
	std::vector<BasicTexture2D*> m_textures;
	std::vector<RenderBuffer*> m_color_rbos;
	RenderBuffer m_depth_rbo;
	FrameBuffer m_fbo_msaa;
	FrameBuffer m_fbo;
	Camera const* m_camera_view;
	SummeadAreaTable m_sat;

	static BasicTexture2D::TextureParm s_parm;
	static int s_number_msaa;
	static int s_shadow_softness;
	static constexpr float SHADOW_RADIUS = 10.0f; // distance from light to shadow center
};
