#pragma once
#include "Scene.h"
#include "BatchRenderer.h"


class BatchRenderScene :public Scene
{
public:
	BatchRenderScene();
	~BatchRenderScene();

	void ImGuiRenderer() override;
	void Update() override;

private:
	const unsigned int mNumberofSprites;
	int mU_MVP; //uniform location

	Matrix<float, 3, 3> mOrtho; //Orthographic Matrix
	TranslationMatrix3 mTranMat;
	ScaleMatrix3 mScaleMat;
	RotationMatrix3 mRotMat;
	Matrix<float, 3, 3> mMVP; //Model view projection

	std::vector<VertexData> mSprites;
	std::unique_ptr<BatchRenderer> mBatchRenderer;
	std::unique_ptr<Renderer> mRenderer;
	std::unique_ptr<Shader> mShader;
};
