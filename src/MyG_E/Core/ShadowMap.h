#pragma once

#include "Foundation/Math/Vector.h"
#include "Core/Texture/BasicTexture2D.h"
#include "Core/FrameBuffer.h"
#include "Core/Camera/Camera.h"

class ShadowMap
{
public:
	explicit ShadowMap(Camera const* camera);
	~ShadowMap();

	void begin() const; // call before draw buffer
	void end() const; // call after draw buffer

	void bind(unsigned int const slot = 0) const; // bind texture
	void unbind() const; // unbind texture

	inline Vector3f get_central_pos() const { return m_camera_view->get_position() + (SHADOW_RADIUS * m_camera_view->get_direction()); }
	inline static constexpr float get_shadow_radius() { return SHADOW_RADIUS; }

	void imgui_renderer();

private:
	BasicTexture2D m_texture;
	FrameBuffer m_depth_map;
	Camera const* m_camera_view;

	static constexpr unsigned int SHADOW_WIDTH = 1024;
	static constexpr unsigned int SHADOW_HEIGHT = 1024;
	static constexpr float SHADOW_RADIUS = 15.0f; // distance from light to shadow center
};
