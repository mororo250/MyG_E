#include "Texture2D.h"

Texture2D::Texture2D()
	:mTranX(384.0f), mTranY(512.0f), mScaleX(1.0f), mScaleY(1.0f), mAngle(0.0f),
	mScaleMat(1.0f, 1.0f), mRotMat(0.0f),
	mOrtho(CreateOrthoMatrix(0.0f, 1024, 768, 0)), mTranMat(512.0f, 384.0f)
{
	float positions[] = {
	-50.0f,  -50.0f , 0.0f, 0.0f, //index 0
	50.0f ,  -50.0f , 1.0f, 0.0f, //index 1
	50.0f ,  50.0f, 1.0f, 1.0f, //index 2
	-50.0f,  50.0f, 0.0f, 1.0f  //index 3
	};

	unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
	};

	GLcall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	
	mVa = std::make_unique<VertexArray>();
	mVb = std::make_unique<VertexBuffer>(positions, 4, 4);
	mIb = std::make_unique<IndexBuffer>(indices, 6);

	mVa->PushLayout(2, GL_FLOAT, GL_FALSE, 0);
	mVa->PushLayout(2, GL_FLOAT, GL_FALSE, 2);
	mVa->AddBuffer(*mVb);
	
	mShader = std::make_unique<Shader>("Shader.shader"),
	mShader->bind();

	Matrix<float, 3, 3> WorldTransform = mScaleMat * mRotMat * mTranMat;
	mMVP = WorldTransform * mOrtho; //Model view projection

	mU_MVP = mShader->GetUniformLocation("u_MVP");
	mShader->SetUniformMatrix3f(mU_MVP, mMVP);

	mTexture = std::make_unique<Texture>("Resources/PS4.PNG");
	mTexture->bind(0);
	mShader->SetUniform1i(mShader->GetUniformLocation("u_texture"), 0);

	mVa->unbind();
	mVb->unbind();
	mIb->unbind();
	mShader->unbind();
}

Texture2D::~Texture2D()
{
}

void Texture2D::ImGuiRenderer()
{

	ImGui::Text("OpenGL");
	ImGui::SliderFloat("Translation X", &mTranX, 0.0f, 1024);
	ImGui::SliderFloat("Translation y", &mTranY, 0.0f, 768);
	ImGui::SliderFloat("Scale X", &mScaleX, 0.0f, 100.0f);
	ImGui::SliderFloat("Scale y", &mScaleY, 0.0f, 100.0f);
	ImGui::SliderFloat("Rotation", &mAngle, -6.28f, 6.28f);
}

void Texture2D::Update()
{
	mRenderer.Clear();

	mTranMat.SetTranX(mTranX);
	mTranMat.SetTranY(mTranY);
	mScaleMat.SetScaleX(mScaleX);
	mScaleMat.SetScaleY(mScaleY);
	mRotMat.SetAngle(mAngle);

	Matrix<float, 3, 3>  WorldTransform = mScaleMat * mRotMat * mTranMat;
	mMVP = WorldTransform * mOrtho;

	mShader->bind();
	mShader->SetUniformMatrix3f(mU_MVP, mMVP);

	mRenderer.Draw(*mVa, *mIb, *mShader);
}
