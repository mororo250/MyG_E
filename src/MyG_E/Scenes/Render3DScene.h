#pragma once

#include "Scene.h"
#include "Core/Renderer.h"
#include "Core/Model3D.h"
#include "Core/FPSCamera.h"
#include "Core/EditCamera.h"
#include "Core/Light.h"

class Render3DScene : public Scene
{
public:
	Render3DScene();
	~Render3DScene();

	void ImGuiRenderer() override;
	void Update() override;

private:
	std::unique_ptr<Shader> mShader;
	std::unique_ptr<Renderer> mRenderer;
	std::unique_ptr<FPSCamera> mFPSCamera;
	std::unique_ptr<EditCamera> mEditCamera;
	std::unique_ptr<Light> mLight;
	std::vector<Model3D> mBuffer;
	std::vector<const char*> mListboxItem;


	Matrix<float, 4, 4> mPersp; //Perspective Matrix
	Matrix<float, 4, 4> mModel;
	Matrix<float, 4, 4> mViewProjection;

	int m_u_Model;
	int m_u_ViewProjection; //MVP uniform location
	int m_u_LightColor; //light uniform location
	int m_u_LightPos;
};