#pragma once

#include "Scene.h"
#include "Core/Renderer3D.h"
#include "Core/Model3D.h"
#include "Core/Camera/FPSCamera.h"
#include "Core/Camera/EditCamera.h"
#include "Core/Light/PointLight.h"
#include "Core\Light\SpotLight.h"
#include "Core\Light\DirectionalLight.h"

class Render3DScene : public Scene
{
public:
	Render3DScene();
	~Render3DScene();

	void ImGuiRenderer() override;
	void Update() override;

private:
	std::unique_ptr<Shader> mShader;
	std::unique_ptr<Shader> mLightShader;
	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<FPSCamera> mFPSCamera;
	std::unique_ptr<EditCamera> mEditCamera;
	std::vector<Light*> m_light_buffer;
	std::vector<Model3D> m_buffer;
	std::vector<const char*> mListboxItem;
	std::vector<const char*> mListboxLight;

	Matrix<float, 4, 4> mPersp; //Perspective Matrix
	Matrix<float, 4, 4> m_model;
	Matrix<float, 4, 4> mViewProjection;

	// Uniform locations
	int m_u_Model;
	int m_u_ViewProjection;
};