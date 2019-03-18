#pragma once
#include "Scene.h"
#include "BatchRenderer.h"
#include "Texture.h"

class BatchRenderScene :public Scene
{
public:
	BatchRenderScene();
	~BatchRenderScene();

	void ImGuiRenderer() override;
	void Update() override;

private:
	const int mMaxSprites;
	int mU_MVP; //uniform location

	Matrix<float, 3, 3> mOrtho; //Orthographic Matrix
	TranslationMatrix3 mTranMat;
	ScaleMatrix3 mScaleMat;
	RotationMatrix3 mRotMat;
	Matrix<float, 3, 3> mMVP; //Model view projection

	std::vector<VertexData2C> mSprites;
	std::unique_ptr<BatchRenderer> mBatchRenderer;
	std::unique_ptr<Renderer> mRenderer;
	std::unique_ptr<Shader> mShader;
	std::unique_ptr<Texture> mTexture;

	//ImGui Variables
	int mNumberofSprites = 0;
	float mGapSize = 0;

};
