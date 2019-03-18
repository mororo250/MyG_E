#include "Sprite.h"

Sprite::Sprite(const float x, const float y, const float Length, const float Hight)
	:mVertexUsage(VertexUsage::Texture2)
{
	mTextureData2d.reserve(4);
	VertexData2T aux;

	aux.Position = { x - Length, y - Hight };
	aux.Texture = { 0.0f, 0.0f };
	mTextureData2d.push_back(aux);

	aux.Position = { x + Length, y - Hight };
	aux.Texture = { 1.0f, 0.0f };
	mTextureData2d.push_back(aux);

	aux.Position = { x + Length, y + Hight };
	aux.Texture = { 1.0f, 1.0f };
	mTextureData2d.push_back(aux);

	aux.Position = { x - Length, y + Hight };
	aux.Texture = { 0.0f, 1.0f };
	mTextureData2d.push_back(aux);
}

Sprite::Sprite(const float x, const float y, const float Length, const float Hight, Vector<float, 4> Color)
	:mVertexUsage(VertexUsage::Color2)
{
	mColorData2d.reserve(4);
	VertexData2C aux;

	aux.Position = { x - Length, y - Hight };
	aux.Color = Color;
	mColorData2d.push_back(aux);

	aux.Position = { x + Length, y - Hight };
	aux.Color = Color;
	mColorData2d.push_back(aux);

	aux.Position = { x + Length, y + Hight };
	aux.Color = Color;
	mColorData2d.push_back(aux);

	aux.Position = { x - Length, y + Hight };
	aux.Color = Color;
	mColorData2d.push_back(aux);
}

Sprite::~Sprite()
{
}

Base * Sprite::GetData()
{
	
	switch (mVertexUsage)
	{
	case VertexUsage::Texture2:
		return mTextureData2d.data();
	case VertexUsage::Color2:
		return mColorData2d.data();
	}
	return nullptr;
}