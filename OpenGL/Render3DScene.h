#pragma once

#include "Scene.h"
#include "Renderer.h"
#include "Model3D.h"
#include "Camera.h"

class Render3DScene : public Scene
{
public:
	Render3DScene();
	~Render3DScene();

	void ImGuiRenderer() override;
	void Update() override;

private:
	//input
	void CameraInteration();
	bool mModeAux;
	float mSpeed;
	Matrix<float, 3, 3> mYaw; //rottion angle x axis
	Matrix<float, 3, 3> mPitch; //rotation angle y axis
	float mSensitivity;
	std::pair<float, float> mMousePos; //currenty mouse possition
	std::pair<float, float> mMouseLastPos; //mouse position in the last frame

	std::unique_ptr<VertexArray>  mVa;
	std::unique_ptr<VertexBuffer> mVb;
	std::unique_ptr<IndexBuffer>  mIb;
	std::unique_ptr<Shader> mShader;
	std::unique_ptr<Renderer> mRenderer;
	std::unique_ptr<Camera> mCamera;
	std::vector<Model3D> mBuffer;

	Matrix<float, 4, 4> mPersp; //Perspective Matrix
	Matrix<float, 4, 4> mMVP; //Model view projection

	int mU_MVP; //uniform location
	Vector<float, 3> mCameraPos;
	Vector<float, 3> mFrontCamera;
};