#include "Render3DScene.h"
#include "imgui.h"

Render3DScene::Render3DScene()
	:mPersp(CreatePerspectiveMatrix(0.0f, 1024.0f, 768.0f, 0.0f, 0.1f, 800.0f)),
	mCube({ 0.0f, 0.0f, 0.0f }, { 50.0f, 50.0f, 50.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }),
	mTrans(mCube.GetPosition()),
	mScale({1.0f, 1.0f, 1.0f}),
	mRot({0.0f, 0.0f, 0.0f})
{
	mView = LookAt({ 0.0f, 0.0f, -50.0f }, mCube.GetPosition(), { 0.0f, 1.0f, 0.0f });

	unsigned int indices[] = {	//back
								0, 1, 2,
								2, 3, 0,
								//right
								1, 5, 6,
								6, 2, 1,
								//front
								7, 6, 5,
								5, 4, 7,
								//left
								4, 0, 3,
								3, 7, 4,
								//bottom
								4, 5, 1,
								1, 0, 4,
								//top
								3, 2, 6,
								6, 7, 3};

	
	GLcall(glEnable(GL_DEPTH_TEST));
	GLcall(glDisable(GL_BLEND));

	mVa = std::make_unique<VertexArray>();
	mVb = std::make_unique<VertexBuffer>(reinterpret_cast<float *>(mCube.GetData()), 8, 7);
	mIb = std::make_unique<IndexBuffer>(indices, 36);

	mVa->PushLayout(3, GL_FLOAT, GL_FALSE, 0);
	mVa->PushLayout(4, GL_FLOAT, GL_FALSE, 3);
	mVa->AddBuffer(*mVb);

	mShader = std::make_unique<Shader>("Color3D.shader");
	mShader->bind();

	mMVP = mView * mPersp; //Model view projection

	mU_MVP = mShader->GetUniformLocation("u_MVP");
	mShader->SetUniformMatrix4f(mU_MVP, mMVP);


	mVa->unbind();
	mVb->unbind();
	mIb->unbind();
	mShader->unbind();

	mRenderer = std::make_unique<Renderer>();
}

Render3DScene::~Render3DScene()
{
	glDisable(GL_DEPTH_TEST);
}

void Render3DScene::ImGuiRenderer()
{
	ImGui::SliderFloat("Translation X", &mTrans[0], 0.0f, 1024.0f);
	ImGui::SliderFloat("Translation y", &mTrans[1], 0.0f, 768.0f);
	ImGui::SliderFloat("Translation z", &mTrans[2], 0.1f, 800.0f);
	ImGui::SliderFloat("Scale X", &mScale[0], 0.0f, 10.0f);
	ImGui::SliderFloat("Scale y", &mScale[1], 0.0f, 10.0f);
	ImGui::SliderFloat("Scale z", &mScale[2], 0.0f, 10.0f);
	ImGui::SliderFloat("Rotation X", &mRot[0],-6.28f, 6.28f);
	ImGui::SliderFloat("Rotation y", &mRot[1], -6.28f, 6.28f);
	ImGui::SliderFloat("Rotation z", &mRot[2], -6.28f, 6.28f);
}

void Render3DScene::Update()
{
	mCube.SetTranslation(mTrans);
	mCube.SetScale(mScale);
	mCube.SetRotation(mRot);

	mShader->bind();
	mShader->SetUniformMatrix4f(mU_MVP, mMVP);
	Matrix<float, 4, 4> WorldTransform = mCube.GetScale() * mCube.GetRotation() * mCube.GetTranslation();
	mMVP = WorldTransform * mView * mPersp; //Model view projection

	GLcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	mRenderer->Draw(*mVa, *mIb, *mShader);
}
