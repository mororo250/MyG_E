#pragma once

#include "Core\Light\Light.h"


class DirectionalLight : public Light
{
public:
	// Position here indicate where to draw the light but doenst make any real diference.
	DirectionalLight(Vector3f const& position,
		Vector3f const& color = { 1.0f, 1.0f, 1.0f },
		Vector3f const& directional = {0.0f, -1.0f, 0.0f}, ShadowMap* shadow_map = nullptr);
	DirectionalLight(DirectionalLight const& other);
	DirectionalLight& operator=(DirectionalLight const& other);
	~DirectionalLight();

	inline Vector3f get_direction() { return m_direction; }
	inline void set_direction(Vector3f const& direction) { m_direction = direction; }

	inline ShadowMap const* get_shadow_map() override { return m_shadow_map.get(); }
	Matrix4x4f get_light_view() override;
	Matrix4x4f get_light_persp() override;

	void imgui_renderer() override;
	void set_uniform(Shader const* shader) override;

	bool is_shadow_caster() const { return (m_shadow_map) ? true : false; }

	static unsigned short get_count() { return s_count; }
	static unsigned short get_count_shadow_caster() { return s_count_shadow_caster; }

private:
	void copy_other(DirectionalLight const& other);

	Vector3f m_direction;
	Vector3f focus_point; // point where camera is looking to.

	std::unique_ptr<ShadowMap> m_shadow_map;

	static unsigned short s_count;
	static unsigned short s_count_shadow_caster;
};
