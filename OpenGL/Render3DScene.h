#pragma once

#include "Scene.h"
#include "Renderer.h"
#include "Model3D.h"
#include "FPSCamera.h"

class Render3DScene : public Scene
{
public:
	Render3DScene();
	~Render3DScene();

	void ImGuiRenderer() override;
	void Update() override;

private:
	std::unique_ptr<VertexArray>  mVa;
	std::unique_ptr<VertexBuffer> mVb;
	std::unique_ptr<IndexBuffer>  mIb;
	std::unique_ptr<Shader> mShader;
	std::unique_ptr<Renderer> mRenderer;
	std::unique_ptr<FPSCamera> mCamera;
	std::vector<Model3D> mBuffer;

	Matrix<float, 4, 4> mPersp; //Perspective Matrix
	Matrix<float, 4, 4> mMVP; //Model view projection

	int mU_MVP; //uniform location
};