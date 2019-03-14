#pragma once

#include "Scene.h"

#include "Renderer.h"
#include "Texture.h"
#include "Math.h"

#include "imgui.h"


class Texture2D : public Scene 
{
public:
	Texture2D();
	~Texture2D();

	void ImGuiRenderer() override;
	void Update() override;

private:
	std::unique_ptr<VertexArray>  mVa;
	std::unique_ptr<VertexBuffer> mVb;
	std::unique_ptr<IndexBuffer>  mIb;
	std::unique_ptr<Shader> mShader;
	std::unique_ptr<Texture> mTexture;
	std::unique_ptr<Renderer> mRenderer;

	Matrix<float, 3, 3> mOrtho; //Orthographic Matrix
	TranslationMatrix3 mTranMat;
	ScaleMatrix3 mScaleMat;
	RotationMatrix3 mRotMat;
	Matrix<float, 3, 3> mMVP; //Model view projection

	int mU_MVP; //uniform location
	float mTranX;
	float mTranY;
	float mScaleX;
	float mScaleY;
	float mAngle;

};