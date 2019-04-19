#pragma once

#include "Renderer.h"
#include "Math.h"
#include <vector>
#include "Sprite.h"

template <class T>
class BatchRenderer
{
public:
	BatchRenderer(unsigned int MaxSprites, unsigned int IndicesCount, unsigned int VertexCount)//IndicesCount - Number of indices per sprite / VertexCount - Number of Vertex per sprite  
		:mMaxSprites(MaxSprites),
		mVertexSize(sizeof(T)),
		mIndicesSize(MaxSprites * IndicesCount),
		mSpriteSize(mVertexSize * VertexCount),
		mBufferSize(MaxSprites * mSpriteSize),
		mRenderer()
	{
		GLcall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		mVao = std::make_unique<VertexArray>();
		mVb = std::make_unique<VertexBuffer>(nullptr, VertexCount * MaxSprites, mVertexSize / sizeof(float), BufferUsage::DYNAMIC);

		mVao->PushLayout(2, GL_FLOAT, GL_FALSE, 0);
		mVao->PushLayout(4, GL_FLOAT, GL_FALSE, 2);
		mVao->AddBuffer(*mVb);


		unsigned int* indices = new unsigned int[mIndicesSize];

		int offset = 0;
		for (unsigned int i = 0; i < mIndicesSize; i += 6)
		{
			indices[i] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		mIb = std::make_unique<IndexBuffer>(indices, mIndicesSize);
	}
	~BatchRenderer()
	{}

	bool isEnoughRoom(unsigned int NumberofSprites) const { return NumberofSprites <= mMaxSprites; }
	//int getPriority() const;

	void begin()
	{
		mVb->map<T>(mBuffer);
		
	}
	void end()
	{
		mVb->unmap();
	}

	void add(std::vector<T>& Sprites)
	{
		unsigned int size = Sprites.size();
		if (size > mBufferSize)
			std::cout << "not enough room" << std::endl;
		else
		{
			memcpy(mBuffer, Sprites.data(), sizeof(T) * size);
			mBuffer += size;
		}
	}
	void add(void* Sprites, unsigned int size)
	{
		memcpy(mBuffer, Sprites, sizeof(T) * size);
		mBuffer += size;
	}
	void Render()
	{
		mRenderer->Clear();
		mVao->bind();
		mIb->bind();
		mRenderer->Draw(*mIb);
		mVao->unbind();
		mIb->unbind();
	}

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
	T* mBuffer;
};