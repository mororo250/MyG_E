#include "BatchRenderScene.h"
#include "imgui.h"


BatchRenderScene::BatchRenderScene()
	:m_max_sprites(1000000),
	mScaleMat(1.0f, 1.0f),
	mRotMat(0.0f),
	mTranMat(0.0f, 0.0f),
	mOrtho(CreateOrthoMatrix(0.0f, 1024, 768, 0)),
	mNumberofSprites(1000),
	mGapSize(1.2)
{
	mBatchRenderer = std::make_unique<BatchRenderer<VertexData2C>>(m_max_sprites, 6, 4);

	mSprites.reserve(m_max_sprites * 4);

	srand(time(NULL));

	float sizeSquare = sqrt((1024.0f * 768.0f) / mNumberofSprites);
	unsigned int line = 1024 / static_cast<unsigned int>(sizeSquare);//number of square in one line
	unsigned int Column = 768 / static_cast<unsigned int>(sizeSquare); //number of square in one column
	Vector<float, 4> Color;
	float x = sizeSquare / 2.0f;
	float y = sizeSquare / 2.0f;
	//create buffer of sprites
	for (unsigned int i = 0; i < Column; i++)
	{
		for (unsigned int j = 0; j < line; j++)
		{
			Color = { rand() % 100 / 100.0f,  rand() % 100 / 100.0f,  rand() % 100 / 100.0f ,  1.0 };
			Sprite sprite(x, y, sizeSquare / 2, sizeSquare / 2, Color);
			VertexData2C* aux = static_cast<VertexData2C*>(sprite.GetData());

			mSprites.push_back(*aux);
			aux++;
			mSprites.push_back(*aux);
			aux++;
			mSprites.push_back(*aux);
			aux++;
			mSprites.push_back(*aux);

			x += (sizeSquare * mGapSize);
		}
		y += (sizeSquare * mGapSize);
		x = sizeSquare / 2.0f;
	}

	mShader = std::make_unique<Shader>("Color.glsl");
	mShader->bind();

	Matrix<float, 3, 3> WorldTransform = mScaleMat * mRotMat * mTranMat;
	mMVP = WorldTransform * mOrtho; //Model view projection

	mU_MVP = mShader->get_uniform_location("u_MVP");
	mShader->set_uniformMatrix3f(mU_MVP, mMVP);

	mBatchRenderer->begin();
	mBatchRenderer->add(mSprites);
	mBatchRenderer->end();


	mShader->unbind();
}

BatchRenderScene::~BatchRenderScene()
{
}

void BatchRenderScene::ImGuiRenderer()
{
	ImGui::SliderInt("Number of sprites", &mNumberofSprites, 0, m_max_sprites);
	ImGui::SliderFloat("Size of the gap", &mGapSize, 0.0f, 3.0f);
}

void BatchRenderScene::Update()
{
	mSprites.clear();
	{
		float sizeSquare = sqrt((1024.0f * 768.0f) / mNumberofSprites);
		unsigned int line = 1024 / static_cast<unsigned int>(sizeSquare);//number of square in one line
		unsigned int Column = 768 / static_cast<unsigned int>(sizeSquare); //number of square in one column
		Vector<float, 4> Color;
		float x = sizeSquare / 2.0f;
		float y = sizeSquare / 2.0f;
		//create buffer of sprites
		for (unsigned int i = 0; i < Column; i++)
		{
			for (unsigned int j = 0; j < line; j++)
			{
				Color = { rand() % 100 / 100.0f,  rand() % 100 / 100.0f,  rand() % 100 / 100.0f ,  1.0 };
				Sprite sprite(x, y, sizeSquare / 2, sizeSquare / 2, Color);
				VertexData2C* aux = static_cast<VertexData2C*>(sprite.GetData());

				mSprites.push_back(*aux);
				aux++;
				mSprites.push_back(*aux);
				aux++;
				mSprites.push_back(*aux);
				aux++;
				mSprites.push_back(*aux);

				x += (sizeSquare * mGapSize);
			}
			y += (sizeSquare * mGapSize);
			x = sizeSquare / 2.0f;
		}
	}

	mBatchRenderer->begin();
	mBatchRenderer->add(mSprites);
	mBatchRenderer->end();

	mShader->bind();
	mShader->set_uniformMatrix3f(mU_MVP, mMVP);

	mBatchRenderer->Render();

	mShader->unbind();
}