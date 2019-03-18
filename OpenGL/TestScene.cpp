#include "TestScene.h"
#include "Sprite.h"

TestScene::TestScene()
:mTranX(0.0f), mTranY(0.0f), mScaleX(1.0f), mScaleY(1.0f), mAngle(0.0f),
mScaleMat(1.0f, 1.0f),
mRotMat(0.0f),
mTranMat(0.0f, 0.0f),
mOrtho(CreateOrthoMatrix(0.0f, 1024, 768, 0))
{
	float positions[] = {
		-50.0f, -50.0f, 1.0f, 0.0f, 0.0f, 1.0f,	//index 0
		50.0f , -50.0f, 1.0f, 0.0f, 0.0f, 1.0f,	//index 1
		50.0f , 50.0f, 1.0f, 0.0f, 0.0f, 1.0f,	//index 2
		-50.0f, 50.0f, 1.0f, 0.0f, 0.0f, 1.0f	//index 3
	};


	unsigned int indices[] = {
		0, 1, 2, 2 ,3, 0
	};

	GLcall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	mVa = std::make_unique<VertexArray>();
	mVb = std::make_unique<VertexBuffer>(positions, 4, 6);
	mIb = std::make_unique<IndexBuffer>(indices, 6);

	mVa->PushLayout(2, GL_FLOAT, GL_FALSE, 0);
	mVa->PushLayout(4, GL_FLOAT, GL_FALSE, 2);
	mVa->AddBuffer(*mVb);

	mShader = std::make_unique<Shader>("Color.shader");
	mShader->bind();

	Matrix<float, 3, 3> WorldTransform = mScaleMat * mRotMat * mTranMat;
	mMVP = WorldTransform * mOrtho; //Model view projection

	mU_MVP = mShader->GetUniformLocation("u_MVP");
	mShader->SetUniformMatrix3f(mU_MVP, mMVP);


	mVa->unbind();
	mVb->unbind();
	mIb->unbind();
	mShader->unbind();

	mRenderer = std::make_unique<Renderer>();

}

TestScene::~TestScene()
{
}

void TestScene::ImGuiRenderer()
{
	ImGui::Text("OpenGL");
	ImGui::SliderFloat("Translation X", &mTranX, 0.0f, 1024);
	ImGui::SliderFloat("Translation y", &mTranY, 0.0f, 768);
	ImGui::SliderFloat("Scale X", &mScaleX, 0.0f, 100.0f);
	ImGui::SliderFloat("Scale y", &mScaleY, 0.0f, 100.0f);
	ImGui::SliderFloat("Rotation", &mAngle, -6.28f, 6.28f);
}

void TestScene::Update()
{
	mRenderer->Clear();

	mTranMat.SetTranX(mTranX);
	mTranMat.SetTranY(mTranY);
	mScaleMat.SetScaleX(mScaleX);
	mScaleMat.SetScaleY(mScaleY);
	mRotMat.SetAngle(mAngle);


	Matrix<float, 3, 3>  WorldTransform = mScaleMat * mRotMat * mTranMat;
	mMVP = WorldTransform * mOrtho;

	mShader->bind();
	mShader->SetUniformMatrix3f(mU_MVP, mMVP);

	mRenderer->Draw(*mVa, *mIb, *mShader);
}
