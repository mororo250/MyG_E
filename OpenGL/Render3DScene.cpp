#include "Render3DScene.h"
#include "Input.h"
#include "imgui.h"
#include <time.h>

Render3DScene::Render3DScene()
	:mPersp(CreatePerspectiveMatrix(45.0f, 1024.0f / 768.0f, 0.1f, 800.0f))
{
	srand(time(NULL));

	std::vector<Vector<float, 4>> ColorBuffer;
	ColorBuffer.reserve(8);
	for (unsigned int i = 0; i < 8; i++)
		ColorBuffer.push_back({ rand() % 100 / 100.0f,  rand() % 100 / 100.0f,  rand() % 100 / 100.0f ,  1.0 });
	//Create the models
	{
		mBuffer.push_back(Model3D({ 0.5, 0.5, 0.5 }, ColorBuffer));
		mBuffer[0].SetTranslation({0.0f, 0.0f, -30.0f}); //bug gets a black screen
		mBuffer.push_back(Model3D({ 1.5f, 1.5f, -10.0f }));
		mBuffer.push_back(Model3D({ -3.0f, 3.0f, -14.0f }));
		mBuffer.push_back(Model3D({ 4.0f, -2.0f, -12.0f }));
		mBuffer.push_back(Model3D({ -6.0f, -6.0f, -20.0f }));
		mBuffer.push_back(Model3D({ 7.0f, 6.0f, -23.0f }));
		mBuffer.push_back(Model3D({ -10.0f, 8.0f, -25.0f }));
		mBuffer.push_back(Model3D({ -10.0f, -8.0f, -25.0f }));
		mBuffer.push_back(Model3D({6.0f, -8.0f, -26.0f }));

		mBuffer.push_back(Model3D({ 1.5f, 1.5f, 10.0f }));
		mBuffer.push_back(Model3D({ -3.0f, 3.0f, 14.0f }));
		mBuffer.push_back(Model3D({ 4.0f, -2.0f, 12.0f }));
		mBuffer.push_back(Model3D({ -6.0f, -6.0f, 20.0f }));
		mBuffer.push_back(Model3D({ 7.0f, 6.0f, -23.0f }));
		mBuffer.push_back(Model3D({ -10.0f, 8.0f, 25.0f }));
		mBuffer.push_back(Model3D({ -10.0f, -8.0f, 25.0f }));
		mBuffer.push_back(Model3D({ 6.0f, -8.0f, 26.0f }));
	}

	mFPSCamera = std::make_unique<FPSCamera>(Vector<float, 3>({ 0.0f, 0.0f, 0.0f }), Vector<float, 3>({0.0f, 0.0f, -1.0f}));
	mEditCamera = std::make_unique<EditCamera>(Vector<float, 3>({ 0.0f, 8.0f, 30.0f }), Vector<float, 3>({ 0.0f, 0.0f, 0.0f }));

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

	mVa = std::make_unique<VertexArray>();
	mVb = std::make_unique<VertexBuffer>(reinterpret_cast<float *>(mBuffer[0].GetData()), 8, 7);
	mIb = std::make_unique<IndexBuffer>(indices, 36);

	mVa->PushLayout(3, GL_FLOAT, GL_FALSE, 0);
	mVa->PushLayout(4, GL_FLOAT, GL_FALSE, 3);
	mVa->AddBuffer(*mVb);

	mShader = std::make_unique<Shader>("Color3D.shader");
	mShader->bind();

	mU_MVP = mShader->GetUniformLocation("u_MVP");


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
	mEditCamera->ImGuiRenderer();
}

void Render3DScene::Update()
{
	mEditCamera->Update();

	GLcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


	for (auto aux : mBuffer)
	{
		mShader->bind();
		mShader->SetUniformMatrix4f(mU_MVP, mMVP);
		mMVP = aux.GetScale() * aux.GetRotation() * aux.GetTranslation() * mEditCamera->GetView() * mPersp; //Model view projection

		mRenderer->Draw(*mVa, *mIb, *mShader);
	}
}
