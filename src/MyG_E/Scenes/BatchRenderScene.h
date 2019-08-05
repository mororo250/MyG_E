#pragma once
#include "Scene.h"
#include "Core/BatchRenderer.h"
#include "Core/Texture.h"

class BatchRenderScene :public Scene
{
public:
	BatchRenderScene();
	~BatchRenderScene();

	void ImGuiRenderer() override;
	void Update() override;

private:
	const int m_max_sprites;
	int mU_MVP; //uniform location

	Matrix<float, 3, 3> mOrtho; //Orthographic Matrix
	TranslationMatrix3 mTranMat;
	ScaleMatrix3 mScaleMat;
	RotationMatrix3 mRotMat;
	Matrix<float, 3, 3> mMVP; //Model view projection

	std::vector<VertexData2C> mSprites;
	std::unique_ptr<BatchRenderer<VertexData2C>> mBatchRenderer;
	std::unique_ptr<Shader> mShader;
	std::unique_ptr<Texture> m_texture;

	//ImGui Variables
	int mNumberofSprites = 0;
	float mGapSize = 0;

};
