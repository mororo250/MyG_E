#pragma once

#include "Renderer.h"

class BatchRenderer
{
public:
	BatchRenderer();
	~BatchRenderer();

	bool isEnoughRoom() const;
	int getPriority() const;

	void add();
	void render();

private:
	unsigned int mMaxNumVertices;
	unsigned int mNumUsed;
	VertexArray mVao;
	VertexBuffer mVb;

};