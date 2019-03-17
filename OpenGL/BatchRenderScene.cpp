#include "BatchRenderScene.h"
#include <time.h>


BatchRenderScene::BatchRenderScene()
	:mNumberofSprites(10000),
	mBatchRenderer(new BatchRenderer(mNumberofSprites, 6, 4)),
	mScaleMat(1.0f, 1.0f), 
	mRotMat(0.0f),
	mTranMat(0, 0),
	mOrtho(CreateOrthoMatrix(0.0f, 1024, 768, 0))
{
	mSprites.reserve(mNumberofSprites * 4);

	srand(time(NULL));

	float sizeSquare = sqrt((1024.0f * 768.0f)/mNumberofSprites); 
	unsigned int line = 1024 / sizeSquare;//number of square in one line
	unsigned int Column = 768 / sizeSquare; //number of square in one column
	Vector<float, 4> Color;
	float offsetX = 0;
	float offsetY = 0;
	VertexData aux;
	//create buffer of sprites
	for (unsigned int i = 0; i < Column; i++)
	{
		for (unsigned int j = 0; j < line; j++)
		{

			Color = { rand() % 1000 / 1000.0f,  rand() % 1000 / 1000.0f,  rand() % 1000 / 1000.0f ,  1.0 };

			aux.Color = Color;
			aux.Position = { offsetX, offsetY };
			mSprites.push_back(aux);

			aux.Color = Color;
			aux.Position = { sizeSquare + offsetX, offsetY };
			mSprites.push_back(aux);

			aux.Color = Color;
			aux.Position = { sizeSquare + offsetX, sizeSquare + offsetY };
			mSprites.push_back(aux);

			aux.Color = Color;
			aux.Position = { offsetX, sizeSquare + offsetY };
			mSprites.push_back(aux);

			offsetX += sizeSquare;
		}
		offsetY += sizeSquare;
	}

	mShader = std::make_unique<Shader>("Color.shader");
	mShader->bind();

	Matrix<float, 3, 3> WorldTransform = mScaleMat * mRotMat * mTranMat;
	mMVP = WorldTransform * mOrtho; //Model view projection

	mU_MVP = mShader->GetUniformLocation("u_MVP");
	mShader->SetUniformMatrix3f(mU_MVP, mMVP);
}

BatchRenderScene::~BatchRenderScene()
{
}

void BatchRenderScene::ImGuiRenderer()
{
}

void BatchRenderScene::Update()
{
	GLcall(glClear(GL_COLOR_BUFFER_BIT));

	mShader->bind();
	mShader->SetUniformMatrix3f(mU_MVP, mMVP);

	mBatchRenderer->begin();
	mBatchRenderer->add(mSprites);
	mBatchRenderer->end();

	mBatchRenderer->Render(*mShader);
}
