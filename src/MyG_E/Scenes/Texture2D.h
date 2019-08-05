#pragma once

#include "Scene.h"

#include "Core/Renderer2D.h"
#include "Core/Texture.h"

#include "Foundation\Math\Vector.h"
#include "Foundation\Math\Matrix.h"

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
	std::unique_ptr<VertexBuffer> m_vb;
	std::unique_ptr<IndexBuffer>  m_ib;
	std::unique_ptr<Shader> mShader;
	std::unique_ptr<Texture> m_texture;
	std::unique_ptr<Renderer> m_renderer;

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