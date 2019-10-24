#pragma once

#include "Core/Renderer3D.h"
#include "Core/Model3D.h"
#include "Core/Camera/FPSCamera.h"
#include "Core/Camera/EditCamera.h"
#include "Core/Light/PointLight.h"
#include "Core\Light\SpotLight.h"
#include "Core\Light\DirectionalLight.h"
#include "Core/SkyBox.h"

class ProjectController : public Layer
{
public:
	ProjectController();
	~ProjectController();

	void update() override;
	void event() override {}
	
	void imgui_renderer() override;
	
	// Light Buffer
	inline void push_light(Light* light) { m_light_buffer.push_back(light); }
	void pop_light(Light* light);
	inline std::vector<Light*> const get_light_buffer() const { return m_light_buffer; }

	// Object Buffer
	inline void push_object(Model3D* object) { m_object_buffer.push_back(object); }
	void pop_object(Model3D* object);
	inline std::vector<Model3D*> const get_object_buffer() const { return m_object_buffer; }

	// Skybox 
	inline void set_skybox(TextureCubMap* texture) { m_skybox.reset(new SkyBox(texture)); }
	inline SkyBox const* get_skybox() const { return m_skybox.get(); }

	// Currente Camera
	void set_camera(Camera* camera);
	inline Camera const* get_camera() const { return m_camera.get(); }

	// perspective matrix:
	void set_perspective_matrix();

	void set_fov(float fov); // Maximum m_fov = 120 and minimum = 30
	inline float get_fov() const { return m_fov; }
private:
	void create_object(std::string const& file_path);
	void create_light(unsigned int type);
	void set_current_shader(unsigned int index);
	void set_current_camera(unsigned int index);

	std::unique_ptr<Shader> m_shader;
	std::unique_ptr<Shader> m_normal_shader;
	std::unique_ptr<Shader> m_light_shader;
	std::unique_ptr<Shader> m_skybox_shader;
	std::unique_ptr<Renderer> m_renderer;
	
	// entities
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<SkyBox> m_skybox;
	std::vector<Light*> m_light_buffer;
	std::vector<Model3D*> m_object_buffer;

	Matrix<float, 4, 4> m_persp_matrix;
	float m_fov; // I am still not sure if this should be a menber of camera or not.....

	unsigned char m_flags; // enum flags

	enum flags
	{
		flags_none =			0,
		flags_draw_lights =		1 << 0,
		flags_draw_normals =	1 << 1,
		flags_draw_skybox =		1 << 2
	};
	enum current_shader
	{
		BLINN_PHONG = 0,
		PHONG = 1
	};
	enum current_camera
	{
		EDIT_CAMERA = 0,
		FPS_CAMERA = 1
	};

	current_shader m_current_shader;
	current_camera m_current_camera;
};
