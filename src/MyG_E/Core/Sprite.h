#pragma once

#include <vector>

#include "Foundation\Math\Vector.h"

struct Base
{
};


struct VertexData2T : public Base
{
	Vector<float, 2> Position;
	Vector<float, 2> Texture;
};


struct VertexData2C : public Base
{
	Vector<float, 2> Position;
	Vector<float, 4> Color;
};

struct VertexData3C : public Base
{
	Vector<float, 3> Position;
	Vector<float, 4> Color;
};

enum class VertexUsage
{
	Texture2, Color2
};

class Sprite
{
public:
	Sprite(const float x, const float y, const float Length, const float Hight);
	Sprite(const float x, const float y, const float Length, const float Hight, Vector<float, 4> Color);
	~Sprite();


	Base* GetData(); //call this class to pass data to glBufferData()

private:
	std::vector<VertexData2C> mColorData2d;
	std::vector<VertexData2T> mTextureData2d;
	const VertexUsage mVertexUsage;
};
