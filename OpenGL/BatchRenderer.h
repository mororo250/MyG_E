#pragma once

#include "Renderer.h"
#include "Math.h"
#include <vector>


struct VertexData 
{
	Vector<float, 2> Position;
	Vector<float, 4> Color;
};

class BatchRenderer
{
public:
	BatchRenderer(unsigned int MaxSprites, unsigned int IndicesCount, unsigned int VertexCount);//IndicesCount - Number of indices per sprite / VertexCount - Number of Vertex per sprite  
	~BatchRenderer();

	bool isEnoughRoom(unsigned int NumberofSprites) const { return NumberofSprites <= mMaxSprites; }
	//int getPriority() const;

	void begin();
	void end();
	void add(std::vector<VertexData>& Sprites);
	void Render(Shader& shader);

private:
	const unsigned int mMaxSprites;
	const unsigned int mVertexSize;
	const unsigned int mIndicesSize;
	const unsigned int mSpriteSize;
	const unsigned int mBufferSize;
	
	
	std::unique_ptr<VertexArray> mVao;
	std::unique_ptr<VertexBuffer> mVb;
	std::unique_ptr<IndexBuffer> mIb;
	std::unique_ptr<Renderer> mRenderer;
	VertexData* mBuffer;
};