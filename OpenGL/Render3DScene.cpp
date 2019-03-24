#include "Render3DScene.h"
#include "imgui.h"
#include <time.h>

Render3DScene::Render3DScene()
	:mPersp(CreateOrthographicMatrix(0.0f, 1024.0f, 768.0f, 0.0f, 0.1f, 800.0f)),
	mTrans({ 512.0f, 384.0f, 0.0f }),
	mScale({200.0f, 200.0f, 200.0f}),
	mRot({0.0f, 0.0f, 0.0f})
{
	srand(time(NULL));

	std::vector<Vector<float, 4>> ColorBuffer;
	ColorBuffer.reserve(8);
	for (unsigned int i = 0; i < 8; i++)
		ColorBuffer.push_back({ rand() % 100 / 100.0f,  rand() % 100 / 100.0f,  rand() % 100 / 100.0f ,  1.0 });

	mCube = std::make_unique<Model3D>(Vector<float, 3>({ 0.5, 0.5, 0.5 }), ColorBuffer);

	mView = LookAt({ 0.0f, 0.0f, -3.0f }, mCube->GetPosition(), { 0.0f, 1.0f, 0.0f });

	unsigned int indices[] = {	//back
								0, 1, 2,
								2, 3, 0,
								//front
								4, 5, 6,
								6, 7, 4,
								//left
								7, 3, 0,
								0, 4, 7,
								//right
								6, 2, 1,
								1, 5, 6,
								//bottom
								0, 1, 5,
								5, 4, 0,
								//top
								3, 2, 6,
								6, 7, 3};

	
	GLcall(glEnable(GL_DEPTH_TEST));
	GLcall(glDisable(GL_BLEND));

	mVa = std::make_unique<VertexArray>();
	mVb = std::make_unique<VertexBuffer>(reinterpret_cast<float *>(mCube->GetData()), 8, 7);
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
	//4ImGui::SliderFloat("Position ClipSpace: (%f, %f, %f)");
	ImGui::SliderFloat("Translation X", &mTrans[0], 0.0f, 1024.0f);
	ImGui::SliderFloat("Translation y", &mTrans[1], 0.0f, 768.0f);
	ImGui::SliderFloat("Translation z", &mTrans[2], -400.1f, 800.0f);
	ImGui::SliderFloat("Scale X", &mScale[0], 0.0f, 1000.0f);
	ImGui::SliderFloat("Scale y", &mScale[1], 0.0f, 1000.0f);
	ImGui::SliderFloat("Scale z", &mScale[2], 0.0f, 1000.0f);
	ImGui::SliderFloat("Rotation X", &mRot[0],-6.28f, 6.28f);
	ImGui::SliderFloat("Rotation y", &mRot[1], -6.28f, 6.28f);
	ImGui::SliderFloat("Rotation z", &mRot[2], -6.28f, 6.28f);
}

void Render3DScene::Update()
{
	mCube->SetTranslation(mTrans);
	mCube->SetScale(mScale);
	mCube->SetRotation(mRot);

	mShader->bind();
	mShader->SetUniformMatrix4f(mU_MVP, mMVP);
	Matrix<float, 4, 4> WorldTransform = mCube->GetScale() * mCube->GetRotation() * mCube->GetTranslation();
	mMVP = WorldTransform * mView * mPersp; //Model view projection

	GLcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	mRenderer->Draw(*mVa, *mIb, *mShader);
}
