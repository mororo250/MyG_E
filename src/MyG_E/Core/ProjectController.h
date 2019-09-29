#pragma once

#include "Core/Renderer3D.h"
#include "Core/Model3D.h"
#include "Core/Camera/FPSCamera.h"
#include "Core/Camera/EditCamera.h"
#include "Core/Light/PointLight.h"
#include "Core\Light\SpotLight.h"
#include "Core\Light\DirectionalLight.h"

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

	// Currente Camera
	inline void set_camera(Camera* camera) { m_camera = camera; }
	inline Camera const* get_camera() const { return m_camera; }

	// perspective matrix:
	void set_perspective_matrix(float aspect_atio);
private:
	void create_object(Shape const shape);
	void create_light(unsigned int type);

	std::unique_ptr<Shader> m_shader;
	std::unique_ptr<Shader> m_light_shader;
	std::unique_ptr<Renderer> m_renderer;
	Camera* m_camera;
	std::vector<Light*> m_light_buffer;
	std::vector<Model3D*> m_object_buffer;

	Matrix<float, 4, 4> m_persp_matrix;
};