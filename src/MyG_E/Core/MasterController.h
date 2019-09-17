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

	void Update();
	void event() {}
	
	void ImGuiRenderer();

private:
	std::unique_ptr<Shader> m_shader;
	std::unique_ptr<Shader> m_light_shader;
	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<Camera> m_camera;
	std::vector<Light*> m_light_list;
	std::vector<Model3D> m_buffer;

	Matrix<float, 4, 4> m_persp_matrix;
};